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

    /* For identification with NickServ */
    nick_pw = "",
    server_pw = "";
};

class client {
public:
    client() { }

    /* Put all these in commands.cpp? */
    void nick(const std::string &nick);
    void join(const std::string chan, const std::string &key);
    //void invite(const std::string &nick, const std::string &chan);
    void message(const std::string target, const std::string message);
    void quite(const std::string &message);
    void loop();


protected:
    config conf;

    void write_handler();
    void read_handler(const std::string &content);

    std::vector<std::function<void (const std::string&)>> read_handlers_;
    connection con;
};

/* ns libircppclient */
}

/* endif LIBIRCPPCLIENT */
#endif