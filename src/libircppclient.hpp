/*
 * Copyright (c) 2015 Tmplt <ttemplate223@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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

    /* Optional */

    /* For identification with NickServ */
    nick_pw   = "",
    server_pw = "";

    bool ssl  = true;
};

class client {
public:
    client() { }

    /* Put all these in commands.cpp? */
    void nick(const std::string &nick);
    void join(const std::string chan, const std::string &key);
    //void invite(const std::string &nick, const std::string &chan);
    void message(const std::string target, const std::string message);

    void quit(const std::string &message);
    void run();


protected:
    config conf;

    void write_handler();
    void read_handler(const std::string &content);

    /*
     * Filled with lambdas at run-time which will decide how
     * to handle certain messages.
     */
    std::vector<std::function<void (const std::string&)>> read_handlers_;
    connection con;
};

/* ns libircppclient */
}

/* endif LIBIRCPPCLIENT */
#endif
