#include "libircppclient.hpp"
#include <iostream>
#include <thread>

/*
 * FIFO:
 * https://stackoverflow.com/questions/1262808/which-stl-container-should-i-use-for-a-fifo
 *
 * std::queue for the interface. Unsure about the container, but chunk allocating would
 * seem to be better, which will expand only if the server/client sends messages faster than
 * the lib can process them. This is already standard for std::queue, though.
 */

namespace libircppclient {

client::client(const config &c)
    : conf_(c)
{
    /* For debugging */
    con.set_ext_read_handler([this](const std::string &content) {
        this->read_handler(content);
    });

    add_read_handler([](const std::string &m) {
        std::cout << m;
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

void client::read_handler(const std::string &content)
{
    /*
     * Iterate through all read handlers which may
     * send a reply back to the server.
     */
    for (read_handler_t &func: read_handlers_) {
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
