#pragma once
#include <string>

namespace irc {

/*
 * It really feels like this isn't the quick-n-simple implementation
 * we're looking for.
 *
 * Contains default values.
 */
struct config {
    std::string
        address = "",
        port    = "6667",
        nick    = "Temeraire",
        user    = "Laurence",

    /* Optional */

        /* For identification with NickServ */
        nick_pw   = "",
        server_pw = "";

    bool ssl = false;
};

}

