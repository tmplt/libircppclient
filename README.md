libircppclient
===============================
A simple-to-use library for communication with IRC servers.

Currently, the current implementation within a program using this library is a `struct` containing the following:

```cpp
struct config {
    /* Default values. */
    std::string
        address = "",
        port    = "6667",
        nick    = "Temeraire",
        user    = "Laurence",

    /* Optional */

        /* For identification with NickServ */
        nick_pw   = "",
        server_pw = "";

    /* Support not yet implemented */
    bool ssl  = false;
};
```

which is simply passed as an argument to `irc::client()` as seen in `simple.cpp`:

```cpp
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
```

Any new ideas for the interaction with the library is welcomed!

Todo
----
* Make it easier to use.
* Implement a FIFO structure for messages from the server.
* Create a way to debug it.
* Colour support.

Design Goals and Plans
------------
* Simple, logical, and easy to use.
* Full compliance with [RFC1459](https://tools.ietf.org/html/rfc1459) and by extension [IRCv3](http://ircv3.net/).
* Unicode support.
* Colour support.
* Thread-safe and whatever C++ libraries should be.
* Debug program with an interactive shell (akin to interactive Python)?

Dependencies
------------
* CMake 3.0
* Boost.asio (tested with >= 1.59.0)
* A compiler with C++14 support, such as Clang or GCC

Thanks to:
----------
- [@muriloadriano](https://github.com/muriloadriano) for the creation of [cleverbot](https://github.com/muriloadriano/cleverbot) -- Boost.asio-based IRC bot under the MIT license.
