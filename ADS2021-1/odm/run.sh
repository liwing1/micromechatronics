#! /bin/sh

cmake -S . -B bin/build
make -C bin/build
cd bin
./odm-test