#!/bin/sh

NIC=vec0
mount proc /proc -t proc
echo "nameserver 8.8.8.8" > /etc/resolv.conf
/sbin/ifconfig lo 127.0.0.1 up
/sbin/ifconfig $NIC 192.168.122.2 up

sleep 5
IPERF=/root/iperf3.static
IPERF=iperf3

echo "===iperf3 forward==="
$IPERF -c 192.168.122.1 -fm
echo "===iperf3 reverse==="
$IPERF -c 192.168.122.1 -R -fm

sh /root/netperf-bench.sh

/sbin/halt -f
