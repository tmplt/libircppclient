#!/usr/bin/env bash

sudo apt-get install build-essential
mkdir -p libs/boost && cd libs/boost

echo "Downloading Boost-$(echo $BOOST_VERSION | sed 's/_/\./g')"
wget "http://downloads.sourceforge.net/project/boost/boost/1.60.0/boost_$BOOST_VERSION.tar.bz2" -O boost.tar.bz2
tar xjf boost.tar.bz2
cd ./boost*

./bootstrap.sh --exec-prefix=/usr/local --with-libraries=system
./b2
sudo ./b2 install >/dev/null # Because we still want eventual errors to print.
cd $PROJECT_DIR

