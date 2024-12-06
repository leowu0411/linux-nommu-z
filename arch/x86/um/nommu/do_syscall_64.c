// SPDX-License-Identifier: GPL-2.0

#include <linux/kernel.h>
#include <linux/ptrace.h>
#include <asm/fsgsbase.h>
#include <asm/prctl.h>
#include <kern_util.h>
#include <sysdep/syscalls.h>
#include <os.h>

static int os_x86_arch_prctl(int pid, int option, unsigned long *arg2)
{
	if (!host_has_fsgsbase)
		return os_arch_prctl(pid, option, arg2);

	switch (option) {
	case ARCH_SET_FS:
		wrfsbase(*arg2);
		break;
	case ARCH_SET_GS:
		wrgsbase(*arg2);
		break;
	case ARCH_GET_FS:
		*arg2 = rdfsbase();
		break;
	case ARCH_GET_GS:
		*arg2 = rdgsbase();
		break;
	default:
		pr_warn("%s: unsupported option: 0x%x", __func__, option);
		break;
	}

	return 0;
}

__visible void do_syscall_64(struct pt_regs *regs)
{
	int syscall;

	syscall = PT_SYSCALL_NR(regs->regs.gp);
	UPT_SYSCALL_NR(&regs->regs) = syscall;

	/* set fs register to the original host one */
	os_x86_arch_prctl(0, ARCH_SET_FS, (void *)host_fs);

	/* save fp registers */
	asm volatile("fxsaveq %0" : "=m"(*(struct _xstate *)regs->regs.fp));

	if (likely(syscall < NR_syscalls)) {
		PT_REGS_SET_SYSCALL_RETURN(regs,
				EXECUTE_SYSCALL(syscall, regs));
	}

	PT_REGS_SYSCALL_RET(regs) = regs->regs.gp[HOST_AX];

	/* handle tasks and signals at the end */
	interrupt_end();

	/* restore fp registers */
	asm volatile("fxrstorq %0" : : "m"((current->thread.regs.regs.fp)));

	/* restore back fs register to userspace configured one */
	os_x86_arch_prctl(0, ARCH_SET_FS,
		      (void *)(current->thread.regs.regs.gp[FS_BASE
						     / sizeof(unsigned long)]));

}
