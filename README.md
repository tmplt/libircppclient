libircppclient
==============
[![Build Status](https://travis-ci.org/Tmplt/libircppclient.svg)](https://travis-ci.org/Tmplt/libircppclient)

An easy-to-use IRC-client based on Boost.asio to send and process data from IRC servers.

Design Goals
------------
* An easy-to-use library to connect to IRC servers and send data back and forth.
* Full compliance with [RFC1459](https://tools.ietf.org/html/rfc1459) and by extension [IRCv3](http://ircv3.net/).
* Unicode support.
* Colour support.
* Thread-safe and whatever C++ libraries should be.

Dependencies
------------
* CMake 3.0
* Boost.asio (tested with >=1.59.0)
* A compiler with C++14 support, such as Clang or GCC

Thanks to:
----------
- [@muriloadriano](https://github.com/muriloadriano) for the creation of [cleverbot](https://github.com/muriloadriano/cleverbot) -- Boost.asio-based IRC bot under the MIT license.
