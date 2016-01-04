[![Github Lience](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/Tmplt/libircppclient/master/LICENSE)
[![Github Issues](https://img.shields.io/badge/lib-issues-brightgreen.svg)](https://github.com/Tmplt/libircppclient/issues)

libircppclient
==============
An easy-to-use IRC-client interface based on Boost.asio to send and receive data from IRC servers.

Design Goals
------------
* An easy-to-use library to connect to IRC servers and send data back and forth.
* Full compliance with [RFC2813](https://tools.ietf.org/html/rfc2813) (or maybe [IRCv3](http://ircv3.net/) is a better idea?).
* Thread-safe and whatever C++ libraries should be.

TODO
----
- [X] It compiles!
- [ ] Add Travis CI building
- [ ] Add basic support
  - [X] Connect to the server (and keep the connection alive)
  - [ ] Join channels
  - [ ] Send messages
- [ ] Full RFC2813/IRCv3 compliance
- [ ] Extensive documentation and code commenting.
- [ ] No "holy shit what was he thinking writing this code?"

Non-goals
---------
* Colour decoding/encoding

Dependencies
------------
* Boost.asio (tested with 1.60.0)
* C++11
