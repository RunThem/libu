#!/usr/bin/env bash

# set -x

argc=${#}
argv=(${@})

gdb -q                                  \
    -ex "set pagination off"            \
    -ex "set print pretty on"           \
    -ex "set print frame-arguments all" \
    -ex "set confirm off"               \
    -ex "source ${path}/gdb.cmd"        \
    -ex "run"                           \
    ${argv}