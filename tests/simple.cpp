#include "../src/libircppclient.hpp"
#include <iostream>
using std::cout;    using std::endl;

namespace irc = libircppclient;

int main(int argc, char *argv[])
{
    irc::config conf;
    conf.address = "localhost";
    conf.port    = "6667";
    conf.nick    = "Temeraire";
    conf.user    = "libircppclient";
    conf.ssl     = false;

    conf.nick_pw   = "";
    conf.server_pw = "";

    irc::client client(conf);

    client.start();
}
