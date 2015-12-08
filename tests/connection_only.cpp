#include "../src/connection.hpp"
#include <iostream>
using std::cout; using std::endl;
using std::endl;

namespace irc = libircppclient;

int main()
{
    cout << "Creating connection object..." << endl;
    irc::connection con;

    assert(!con.alive());
}