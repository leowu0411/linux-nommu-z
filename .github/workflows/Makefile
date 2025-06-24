SRCS := clone.c futex.c nullptr.c ctest.c stackp.c do_getpid.c init_array.c entropy.c locale.c vdso_test.c pthread.c test-signal-restore.c
BKUP := exit.c noop.c
TARGETS := $(SRCS:.c=)

CFLAGS_exit = -nostdlib -pie -fPIE -fomit-frame-pointer
CFLAGS += -g -O0
all: ${TARGETS}

tests: ${TARGETS}
	set +x
	./clone host
	./futex
	./nullptr 1 0 || RET=$$? ; echo "code="$$RET && if [ "$$RET" -ne "139" ] ; then false ; fi
	./nullptr 1 1 || RET=$$? ; echo "code="$$RET && if [ "$$RET" -ne "11" -a "$$RET" -ne "139" ] ; then false ; fi
	./nullptr 0 0 || RET=$$? ; echo "code="$$RET && if [ "$$RET" -ne "139" ] ; then false ; fi
	./nullptr 0 1 || RET=$$? ; echo "code="$$RET && if [ "$$RET" -ne "11" -a "$$RET" -ne "139" ] ; then false ; fi
	./nullptr
	echo "test" | ./ctest
	./stackp
	./entropy 10000
	./locale
	./vdso_test -c 10000
	./test-signal-restore
	./pthread
	./pthread 100
	./pthread 100 1
	set -x

clean:
	rm -f ${TARGETS}
