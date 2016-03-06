#include "../src/libircppclient.hpp"
#include <iostream>
using std::cout;    using std::endl;

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

    try {
        irc::client client(conf);
        client.start();
    } catch (const std::exception &e) {
        cout << "Exception thrown: " << e.what() << endl;
    } catch (const boost::system::error_code &e) {
        cout << "Boost error: " << e.message() << endl;
    }
}
