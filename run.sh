#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <day>"
    exit 1
fi

set -xe

gcc -ggdb3 -Wall -Wextra -O3 -I c/ c/$1.c
time ./a.out
