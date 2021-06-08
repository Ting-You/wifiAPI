#!/bin/sh
mkdir _install
make clean 
make PREFIX=$(PWD)/_install
make install
