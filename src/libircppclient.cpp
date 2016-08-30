#include "libircppclient.hpp"
#include "util.hpp"
#include <string>
#include <iostream>
#include <thread>
#include <stdexcept>

namespace irc {

client::client(const config &conf)
    : conf_(conf), con_(conf_.ssl)
{
    std::string ret = validate_conf(conf);

    if (!ret.empty())
        throw std::invalid_argument("Configuration error: " + ret);

    con_.set_addr(conf.address);
    con_.set_port(conf.port);

    /*
     * "Bind" the external read handlers from connection to
     * read_handler() here. Otherwise, you'd need to fiddle
     * with connection.hpp inclusion.
     */
    con_.set_ext_read_handler([this](const std::experimental::string_view &content) {
        this->read_handler(content);
    });
}

std::string client::validate_conf(const config &c)
{

    /* Address checking. */
    if (c.address.empty()) {
        return "the address is empty.";
    } else {
        std::string ret = util::valid_addr(c.address);

        if (!ret.empty())
            return "invalid address, reason: " + ret;
    }

    /* Port checking. */
    if (c.port.empty())
        return "port is empty.";

    else if (!util::is_integer(c.port))
        return "port contains one of more non-integers.";

    return "";
}

void client::initialize()
{
    con_.write("NICK " + conf_.nick);
    con_.write("USER " + conf_.user + " 0 * :" + conf_.user);
}

void client::start()
{
    con_.connect();

    /*
     * These writes will be put in the io_service's
     * queue until a complete connection has been made.
     */
    initialize();
    con_.run();
}

void client::stop()
{
    /*
     * Haven't I forgotten something?
     * Some async function that needs terminating, maybe?
     */
    con_.stop_ping();
    con_.stop();
}

void client::read_handler(const std::experimental::string_view &content)
{
    for (read_handler_t &func: read_handlers_) {
        func(content);
    }
}

void client::add_read_handler(std::function<void(const std::experimental::string_view &)> func)
{
    read_handlers_.emplace_back(func);
}

void client::raw_cmd(const std::experimental::string_view &content)
{
    if (content.empty())
        throw std::invalid_argument("content is empty.");

    con_.write(content.data());
}

/* ns irc */
}

