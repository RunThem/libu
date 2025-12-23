#!/usr/bin/env bash

set -x

find . -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i -style=File {} \;