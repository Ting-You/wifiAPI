#!/bin/sh

mkdir _install 
make clean
make CC=arm-hisiv600-linux-gcc && make install
