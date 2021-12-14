#!/bin/sh

if [ $# -ne 1 ]
then
    echo "Please input day as argument"
    exit 1
fi

set -xe

cp python/template.py python/$1.py
cp c/template.c c/$1.c
touch input/$1.txt
touch input/$1s.txt

code python/$1.py
code c/$1.c
code input/$1.txt
code input/$1s.txt