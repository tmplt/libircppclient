#!/bin/bash

# It's your fault I'm doing this Travis

mkdir lib_boost && cd lib_boost
wget "http://downloads.sourceforge.net/project/boost/boost/1.60.0/boost_1_60_0.tar.bz2"
tar xjf boost_1_60_0.tar.bz2
cd ./boost_1_60_0

# Build the bjam binary
./bootstrap.sh --exec-prefix=/usr/local --with-libraries=system

# Execute it
./bjam
sudo /.bjam install
