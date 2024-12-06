// SPDX-License-Identifier: GPL-2.0

#include <signal.h>
#include <kern_util.h>
#include <os.h>
#include <sysdep/mcontext.h>
#include <sys/ucontext.h>
#include <as-layout.h>

void sigsys_handler(int sig, struct siginfo *si,
		    struct uml_pt_regs *regs, void *ptr)
{
	mcontext_t *mc = (mcontext_t *) ptr;

	/* hook syscall via SIGSYS */
	set_mc_sigsys_hook(mc);
}

void arch_sigsegv_handler(int sig, struct siginfo *si, void *ptr)
{
	mcontext_t *mc = (mcontext_t *) ptr;

	/* !MMU specific part; detection of userspace */
	if (mc->gregs[REG_RIP] > uml_reserved &&
	    mc->gregs[REG_RIP] < high_physmem) {
		/* !MMU: force handle signals after rt_sigreturn() */
		set_mc_userspace_relay_signal(mc);
	}
}
