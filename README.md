libircppclient
==============
An interface based on Boost.asio to send and receive data from IRC servers.

About
-----
* License: zlib License
* Bug reports: [https://github.com/Tmplt/libircppclient/issues](Github issues)

Design Goals
------------
* An easy library to connect to IRC servers and send data back and forth
* Small but useful; do one thing and do it well
* Full compliance with [RFC2813](https://tools.ietf.org/html/rfc2813)
* Thread-safe and whatever C++ libraries should be

Non-goals
---------
* Colour decoding/encoding

Dependencies
------------
* Boost.asio (tested with 1.59.0)
