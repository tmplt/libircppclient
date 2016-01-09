#ifndef LIBIRCPPCLIENT_HPP
#define LIBIRCPPCLIENT_HPP

#include "connection.hpp"
#include <vector>

namespace libircppclient {

/*
 * It really feels like this isn't the quick-n-simple implementation
 * we're looking for.
 */
struct config {
    /* Default values. */
    std::string
    address = "",
    port    = "6667",
    nick    = "Temeraire",
    user    = nick,

    /* Optional */

    /* For identification with NickServ */
    nick_pw   = "",
    server_pw = "";

    bool ssl  = true;
};

class client {
public:
    client(const config &c);

    void nick(const std::string &nick);
    void join(const std::string chan, const std::string &key);
    void msg(const std::string target, const std::string message);
    void quit(const std::string &message);

    /*
     * As this lib is not feature-done, yet:
     * Send the given content directly to the server
     * as a command.
     */
    void raw_cmd(const std::string &content);

    void start();
    void stop()
    {
        /*
         * Haven't I forgotten something?
         * Some async function that needs terminating, maybe?
         */
        con.stop_ping();
        con.stop();
    }

    /* Add a read_handler that may not exist in this class. */
    void add_read_handler(read_handler_t func);

protected:
    config conf_;

    /*
     * Filled with lambdas at run-time which will decide how
     * to handle certain messages from the server.
     */
    void read_handler(const std::string &content);

    std::vector<read_handler_t> read_handlers_;

    connection con;

private:
    void initialize();
};

/* ns libircppclient */
}

/* endif LIBIRCPPCLIENT */
#endif
