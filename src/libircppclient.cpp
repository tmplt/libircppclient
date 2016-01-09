#include "libircppclient.hpp"
#include <iostream>
#include <thread>

namespace libircppclient {

client::client(const config &c)
    : conf_(c)
{
    /*
     * Use functions from this class as the read and
     * write handlers.
     */

    con.set_read_handler([this](const std::string &content) {
        this->read_handler(content);
    });

    add_read_handler([](const std::string &m) {
        std::cout << m;
    });

    /* fail-safe ping, in case the ping thread fails */
    add_read_handler([&](const std::string &m) {
        std::istringstream iss(m);
        std::string command;

        iss >> command;

        if (command == "PING") {
            raw_cmd("PONG :" + conf_.address);
        }

        /*
         * Might have to create a rather inclusive lambda
         * to handle cases such as from-server PINGs,
         * [...], and at last, if the message passes
         * the if-statements, put it into the FIFO.
         *
         * But that doesn't make the function non-worthy
         * of a name, which, it being a lamdba, implies...
         */
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

    /*
     * We can create two fifo object that handles messages sent
     * and received from the server. That structure makes this function
     * quite useless, however, the protocol specifies that it is better
     * that the client PINGs, rather than the server.
     *
     * So... here's a though: rename this to "connection/ping/loop_handler"
     * and make it send a PING to the server every 60 seconds. Sleep with
     * chrono or something.
     *
     * Then, course, present the fifo object/streams to the top level.
     */

    /*
     * FIFO:
     * https://stackoverflow.com/questions/1262808/which-stl-container-should-i-use-for-a-fifo
     *
     * std::queue for the interface. Unsure about the container, but chunk allocating would
     * seem to be better, which will expand only if the server/client sends messages faster than
     * the lib can process them. This is already standard for std::queue, though.
     */
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
