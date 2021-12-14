#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <day>"
    exit 1
fi

set -xe

gcc -ggdb3 -O3 -I c/ c/$1.c
./a.out