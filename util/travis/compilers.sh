#!/usr/bin/env bash

## The GNU toolchain..
sudo apt-get install -qq g++${CVERSION}

if [ "$CXX" = "g++" ]; then
    export CXX="g++-${CVERSION}" CC="gcc-${CVERSION}"
fi

export  CC=`which $CC`
export CXX=`which $CXX`

## and the Clang toolchain..
sudo apt-get install clang-${CLANG_VERSION}

export  CC=`which clang-${CLANG_VERSION}`
export CXX=`which clang++-${CLANG_VERSION}`

