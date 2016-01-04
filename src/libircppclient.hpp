#ifndef LIBIRCPPCLIENT_HPP
#define LIBIRCPPCLIENT_HPP

#include "connection.hpp"
#include <vector>

namespace libircppclient {

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
     * As this lib is not feature done, yet:
     * Send the given content directly to the server
     * as a command.
     */
    void raw_cmd(const std::string &content);

    void start();
    void stop()
    {
        /* Haven't I forgotten something?
         * Some async function that needs terminating, maybe?
         */
        con.close();
    }

    void add_read_handler(std::function<void (const std::string&)> func);

protected:
    config conf_;

    void write_handler();
    void read_handler(const std::string &content);

    /*
     * Filled with lambdas at run-time which will decide how
     * to handle certain messages from the server.
     */
    std::vector<std::function<void (const std::string&)>> read_handlers_;

    connection con;

private:
    void initialize();
};

/* ns libircppclient */
}

/* endif LIBIRCPPCLIENT */
#endif
