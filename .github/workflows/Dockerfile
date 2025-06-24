FROM alpine:3.20.3

ARG MODE=
ADD vmlinux /root/.
ADD inittab /etc/inittab
RUN mkdir /etc/init.d
ADD rcS /etc/init.d/.
RUN sed "s/\'/\"/g" /etc/profile > /.profile

ADD bench.sh /
ADD iperf3.sh /root/
ADD netperf-bench.sh /root/
ADD iperf3.static /root/
ADD do_getpid /root/.
ADD clone /root/.
ADD futex /root/.
ADD nullptr /root/.
ADD ctest /root/.
ADD stackp /root/.
ADD init_array /root/.
ADD entropy /root/.
ADD locale /root/.
ADD vdso_test /root/.
ADD pthread /root/.
ADD test-signal-restore /root/.
ADD Makefile /root/.
ADD lmbench2/ /lmbench2
ADD lmbench_run.sh /lmbench2/bin/x86_64-linux-gnulibc1/
ADD netperf /root/.


RUN apk update && apk add utmps-libs libtirpc curl make iperf3

RUN if [[ ${MODE} == "um-nommu" ]]; then mkdir -p setup && cd setup && curl -L -o output.zip \
    'https://gitlab.alpinelinux.org/thehajime/aports/-/jobs/1641821/artifacts/download?file_type=archive' \
    && unzip output.zip ; fi
RUN if [[ ${MODE} == "um-nommu" ]]; then apk add --allow-untrusted \
   setup/packages/main/x86_64/busybox-nommu-1.36.1-r29.apk \
   setup/packages/main/x86_64/musl-nommu-1.2.5-r0.apk &&  rm -rf setup; fi

ENTRYPOINT ["/root/vmlinux", "vec0:transport=tap,ifname=tap100,depth=128,gro=1", "root=/dev/root", "rootflags=/", "rootfstype=hostfs", "rw", "mem=1024m", "loglevel=0", "init=/sbin/init"]
