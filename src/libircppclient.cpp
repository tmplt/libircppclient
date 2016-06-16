#include "libircppclient.hpp"
#include "general.hpp"
#include <string>
#include <iostream>
#include <thread>
#include <stdexcept>

/*
 * FIFO:
 * https://stackoverflow.com/questions/1262808/which-stl-container-should-i-use-for-a-fifo
 *
 * std::queue for the interface. Unsure about the container, but chunk allocating would
 * seem to be better, which will expand only if the server/client sends messages faster than
 * the lib can process them. This is already standard for std::queue, though.
 */

namespace irc {

client::client(const config &conf)
    : conf_(conf)
{
    std::string ret = validate(conf);

    if (!ret.empty())
        throw std::invalid_argument("Config error: " + ret);

    con.set_addr(conf.address);
    con.set_port(conf.port);

    /*
     * "Bind" the external read handlers from connection to
     * read_handler() here. Otherwise, you'd need to fiddle
     * with connection.hpp inclusion.
     */
    con.set_ext_read_handler([this](const std::string &content) {
        this->read_handler(content);
    });
}

std::string client::validate(const config &c)
{
    using std::string;

    /* Address handling. */

    if (c.address.empty())
        return "the address is empty.";

    else {
        string ret = gen::valid_addr(c.address);

        if (!ret.empty())
            return "invalid address, reason: " + ret;
    }

    /* Port handling. */

    if (c.port.empty())
        return "port is empty.";

    else if (!gen::is_integer(c.port))
        return "port contains one of more non-integers.";

    return "";
}

void client::initialize()
{
    con.write("NICK " + conf_.nick);
    con.write("USER " + conf_.user + " 0 * :" + conf_.user);
}

void client::start()
{
    //con.connect(conf_.address, conf_.port, conf_.ssl);
    con.connect();
    initialize();
    con.run();
}

void client::stop()
{
    /*
     * Haven't I forgotten something?
     * Some async function that needs terminating, maybe?
     */
    con.stop_ping();
    con.stop();
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
    if (content.empty())
        throw std::invalid_argument("content is empty.");

    con.write(content);
}

/* ns irc */
}
