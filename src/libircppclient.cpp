#include "libircppclient.hpp"
#include <iostream>

namespace libircppclient {

client::client(const config &c)
    : conf_(c)
{
    /*
     * Use functions from this class as the read and
     * write handlers.
     */
    con.set_write_handler([this]() { this->write_handler(); });

    con.set_read_handler([this](const std::string &content) {
        this->read_handler(content);
    });

    add_read_handler([](const std::string &m) {
        std::cout << m;
    });

    add_read_handler([&](const std::string &m) {
        std::istringstream iss(m);
        std::string command;

        iss >> command;

        if (command == "PING") {
            raw_cmd("PONG :" + conf_.address);
        }
    });
}

void client::initialize()
{
    con.write("NICK " + conf_.nick);
    con.write("USER " + conf_.user + " 0 * :" + conf_.user);
}

void client::start()
{
    con.connect(conf_.address, conf_.port);
    initialize();
    con.run();
}

void client::write_handler()
{
    /* this is gonna loop a lot... */
}

void client::read_handler(const std::string &content)
{
    /*
     * Iterate through all read handlers which may
     * send a reply back to the server.
     */
    for (auto func: read_handlers_) {
        func(content);
    }
}

void client::add_read_handler(std::function<void(const std::string &)> func)
{
    read_handlers_.push_back(func);
}

void client::raw_cmd(const std::string &content)
{
    con.write(content);
}

/* ns libircppclient */
}
