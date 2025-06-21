// SPDX-License-Identifier: GPL-2.0

#include <linux/kernel.h>
#include <linux/ptrace.h>
#include <kern_util.h>
#include <sysdep/syscalls.h>
#include <os.h>

__visible void do_syscall_64(struct pt_regs *regs)
{
	int syscall;

	syscall = PT_SYSCALL_NR(regs->regs.gp);
	UPT_SYSCALL_NR(&regs->regs) = syscall;

	if (likely(syscall < NR_syscalls)) {
		PT_REGS_SET_SYSCALL_RETURN(regs,
				EXECUTE_SYSCALL(syscall, regs));
	}

	PT_REGS_SYSCALL_RET(regs) = regs->regs.gp[HOST_AX];

	/* handle tasks and signals at the end */
	interrupt_end();
}
