#include "../src/libircppclient.hpp"
#include <iostream>
using std::cout;    using std::endl;

namespace irc = libircppclient;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << "[Error] at least an address argument is required" << endl;
        return 1;
    }

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
}
