// SPDX-License-Identifier: GPL-2.0
#include <sys/ucontext.h>
#define __FRAME_OFFSETS
#include <asm/ptrace.h>
#include <sysdep/ptrace.h>
#include <sysdep/mcontext.h>
#include <sysdep/syscalls.h>

static void __userspace_relay_signal(void)
{
	/* XXX: dummy syscall */
	__asm__ volatile("call *%0" : : "r"(__kernel_vsyscall), "a"(39) :);
}

void set_mc_userspace_relay_signal(mcontext_t *mc)
{
	mc->gregs[REG_RIP] = (unsigned long) __userspace_relay_signal;
}

void set_mc_sigsys_hook(mcontext_t *mc)
{
	mc->gregs[REG_RCX] = mc->gregs[REG_RIP];
	mc->gregs[REG_RIP] = (unsigned long) __kernel_vsyscall;
}
