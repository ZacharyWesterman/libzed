#!/usr/bin/env sh

CC="$1"

$CC -v --help 2>/dev/null | sed -n '/^ *-std=\([^<][^ ]\+\).*/ {s//\1/p}' | grep 'c++[1-8][0-9]' | tail -1 | tr -d .
