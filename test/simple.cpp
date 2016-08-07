/*
 * A simple client utitlizing libircppclient which
 * simply connects to a server, maintains the connection,
 * and prints everything received in its raw form.
 */

#include "../src/libircppclient.hpp"
#include <string>
#include <iostream>
using std::cout;    using std::endl;
using std::cerr;

void print_usage()
{
    cout << "Usage: simple <host> [[+]<port>]" << endl
         << "       '+' for SSL."              << endl
         << "       port defaults to 6667."    << endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        print_usage();
        return -1;
    }

    irc::config conf;
    conf.address = argv[1];

    std::string port = (argv[2] ? argv[2] : "6667");

    if (port[0] == '+') {
        conf.ssl  = true;

        /* Remove the SSL sign. */
        conf.port = port.erase(0,1);
    }

    else {
        conf.ssl  = false;
        conf.port = port;
    }

    if (conf.ssl)
        cout << "[debug] using SSL" << endl;

    conf.nick    = "libircppbot";
    conf.user    = "libircppclient";

    conf.nick_pw   = "";
    conf.server_pw = "";

    try {
        irc::client client(conf);

        client.add_read_handler([](const std::string &m) {
            cout << m;
        });

        client.start();
    }

    catch (const std::exception &e) {
        cerr << "Panic! " << e.what() << endl;
        return -1;
    }

    catch (const boost::system::error_code &e) {
        cerr << "Boost error: " << e.message() << endl;
        return -1;
    }

    return 0;
}

