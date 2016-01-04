/*
 * This test will connect to the server, wait until a PING
 * message has been received, and reply to it.
 */

#include "../src/connection.hpp"
#include <iostream>
using std::cout; using std::endl;
using std::endl;

namespace irc = libircppclient;

int main()
{
    irc::connection con;

    if (con.is_alive()) {
        cout << "The connection is initialized as alive?" << endl;
        return 1;
    }

    //con.set_write_handler([this]() { this->write_handler(); });

    con.connect("localhost", "6667");
    cout << "The connection is... " << (con.is_alive() ? "alive" : "dead") << endl;

    con.run();

}
