#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <ucontext.h>
#include <stdlib.h>

static void segv_handler (int cause, siginfo_t * info, void *uap)
{
	//For test. Never ever call stdio functions in a signal handler otherwise*/
	printf ("SIGSEGV handled\n");
	printf ("SIGSEGV raised at address 0x%lx\n", (unsigned long)info->si_addr);
	ucontext_t *context = uap;
	/*On my particular system, compiled with gcc -O2, the offending instruction
	  generated for "*f = 16;" is 6 bytes. Lets try to set the instruction
	  pointer to the next instruction (general register 14 is EIP, on linux x86) */
	context->uc_mcontext.gregs[16] += 14;
	//alternativly, try to jump to a "safe place"
	//context->uc_mcontext.gregs[14] = (unsigned int)safe_func;
	exit(cause);
}

/* XXX: this code doesn't work as 2nd fprintf() causes SEGV
 * (probably) due to alignment issue of xmm0/rsp register
 *
 * https://stackoverflow.com/questions/5397041/getting-the-saved-instruction-pointer-address-from-a-signal-handler
 */
static void signal_segv(int signum, siginfo_t *info, void *ptr)
{
	static const char *si_codes[3] = {"", "SEGV_MAPERR", "SEGV_ACCERR"};
	int i, f = 0;
	ucontext_t *ucontext = (ucontext_t*)ptr;
	void **bp = 0;
	void *ip = 0;

	fprintf(stderr, "Segmentation Fault!\n");
	fprintf(stderr, "info.si_signo = %d\n", signum);
	fprintf(stderr, "info.si_errno = %d\n", info->si_errno);
	fprintf(stderr, "info.si_code  = %d (%s)\n", info->si_code, si_codes[info->si_code]);
	fprintf(stderr, "info.si_addr  = %p\n", info->si_addr);
	for(i = 0; i < NGREG; i++)
		fprintf(stderr, "reg[%02d]       = 0x%016llx\n", i, ucontext->uc_mcontext.gregs[i]);

	ucontext->uc_mcontext.gregs[16] += 14;
	exit(signum);
}

int main (int argc, char *argv[])
{
	char *ptr = NULL;
	struct sigaction sa;
	int *f = NULL;

	if (argc != 3) {
		printf("%s [nullptr or raise] [handler or not]\n", argv[0]);
		return 0;
	}

	if (atoi(argv[2]) >= 1) {
		printf("register handler\n");
		if (atoi(argv[2]) == 1)
			sa.sa_sigaction = segv_handler;
		else
			sa.sa_sigaction = signal_segv;
		sigemptyset (&sa.sa_mask);
		sa.sa_flags = SA_SIGINFO;
		if (sigaction (SIGSEGV, &sa, 0)) {
			perror ("sigaction");
			return -1;
		}
	}

	if (atoi(argv[1]) == 0)
		memcpy(ptr, 0, 8);
	else {
		printf("raising signal\n");
		raise(SIGSEGV);
	}
	return 0;
}
