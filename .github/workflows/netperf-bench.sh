#!/bin/sh

PSIZES="64 128 256 512 1024 1500 65507"
TESTNAMES="TCP_STREAM TCP_MAERTS"
DEST_ADDR=${1:-"192.168.122.1"}
NETPERF=${2:-/root/netperf}

sudo /sbin/sysctl -w net.ipv4.tcp_wmem="40960 873800 1677721600"
sudo /sbin/sysctl -w net.ipv4.tcp_rmem="40960 873800 1677721600"

for size in $PSIZES
do
    for test in $TESTNAMES
    do
	echo "== netperf ($size-$test)  =="
	$NETPERF -H $DEST_ADDR -t $test -- -o THROUGHPUT,THROUGHPUT_UNITS,LOCAL_SEND_SIZE,COMMAND_LINE -m "$size,$size"

    done
done
