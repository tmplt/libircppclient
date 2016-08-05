libircppclient
==============

A simple-to-use library for communication with IRC servers.

Presently, this library relies on Boost.Asio — and a bit on regular
Boost, this is subject to change to std::asio, once c++20 is the latest
standard.

Usage
-----

The current implementation within a program using this library is a
``struct`` containing the following:

.. code:: cpp

    struct config {
        /* Default values. */
        std::string
            address = "irc.example.tld",
            port    = "6697",
            nick    = "Temeraire",
            user    = "Laurence",

        /* Optional */

            /* For identification with NickServ */
            nick_pw   = "ech!",
            server_pw = "";

        bool ssl  = true;
    };

which is simply passed as an argument to ``irc::client()`` as seen in
``tests/simple.cpp``:

.. code:: cpp

    irc::config conf;
    conf.address = argv[1];
    conf.port    = (argv[2] ? argv[2] : "6667");
    conf.nick    = "libircppbot";
    conf.user    = "libircppclient";
    conf.ssl     = false;

    conf.nick_pw   = "";
    conf.server_pw = "";

    irc::client client(conf);

    client.start();

Design Goals and Plans
----------------------

-  Simple, logical, and easy to use.
-  Full compliance with
   `RFC1459 <https://tools.ietf.org/html/rfc1459>`__ and by extension
   `IRCv3 <http://ircv3.net/>`__.
-  Unicode support.
-  Colour support.
-  Thread-safe and whatever C++ libraries should be.

Dependencies
------------

-  CMake 3.0
-  Boost and Boost.Asio (tested with >= 1.59.0)
-  A compiler with C++11 support, such as Clang or GCC

Thanks to:
----------

-  [@muriloadriano](https://github.com/muriloadriano) for the creation
   of `cleverbot <https://github.com/muriloadriano/cleverbot>`__ —
   Boost.asio-based IRC bot under the MIT license.
-  [@jrosdahl](https://github.com/jrosdahl) for
   `miniircd <https://github.com/jrosdahl/miniircd>`__ — A (very) simple
   IRC server, which prints server events in its raw form.
