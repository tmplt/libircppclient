## About
libircppclient is an IRC client library based on Boost.Asio and experimental
features from C++17.

While Boost.Asio is a current dependency,
it's planned for removal in *some* future version.
Either we'll get `std::asio` from C++20,
or I'll just have to learn sockets.

libircppclient is a work in progress currently on hold.
But I might need it in [bookwyrm](https://github.com/tmplt/bookwyrm).

## Usage

The current implementation within a program using this library is a
`struct` containing the following:

```cpp
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
```
This struct is passed as an argument to `irc::client()` as shown in
`test/simple.cpp`:

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

## Design Goals and Plans
- Compliance with [RFC2812](https://tools.ietf.org/html/rfc2812) and by extension [IRCv3](http://ircv3.net/).
- Unicode support.
- Colour support.

## Code Style
Syntax-wise,
generally follow [the Linux kernel coding style](https://www.kernel.org/doc/Documentation/CodingStyle) except when it comes to indention,
which is four (4) spaces in width.
Tabs are not allowed.

Some parts of the LKCS might be harder to read/understand in C++.
Feel free to deviate from the standard if it improves code quality.

## Dependencies
- CMake 3.0
- Boost.Asio (tested with >= 1.59.0)
- A compiler with C++17 support

## License
libircppclient is distributed under the MIT license.

You can find the license in the root of this repository in the file `LICENSE.md`

## Thanks to:
- [muriloadriano](https://github.com/muriloadriano>) for the creation of [cleverbot](https://github.com/muriloadriano/cleverbot)
- [jrosdahl](https://github.com/jrosdahl) for [miniircd](https://github.com/jrosdahl/miniircd)
- various people on various IRC channels
