#!/usr/bin/env bash

set -x

./build/linux/x86_64/debug/dev.c &>/dev/null &
pid=$!

sleep 1

sudo perf record -g -o perf.data -p ${pid} -- sleep 10
sudo perf script -i perf.data &>perf.unfold
./build/FlameGraph/stackcollapse-perf.pl perf.unfold &>perf.folded
./build/FlameGraph/flamegraph.pl perf.folded >perf.svg

rm -f perf.data perf.unfold perf.folded
