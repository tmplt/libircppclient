#ifndef LIBIRCPPCLIENT_HPP
#define LIBIRCPPCLIENT_HPP

#include "connection.hpp"
#include <vector>

namespace irc {

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
    user    = "Laurence",

    /* Optional */

    /* For identification with NickServ */
    nick_pw   = "",
    server_pw = "";

    /* Support not yet implemented */
    bool ssl  = true;
};

class client {
public:
    client(const config &c);

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

    /*
     * In all commands where a channel name must/may be passed,
     * the channel prefix [#|&|...] must be included.
     */

// Connection registration

    /* Set nickname */
    void nick(const std::string &nick);

    // Password message

    // User message

    // Server message

    // Operator message

    /*
     * Disconnect from the server with an optional quit message.
     * Also stops the irc::client.
     */
    void quit(const std::string &message = "");

    // Server quit message

// Channel operations

    /* Join channel(s) */
    void join(const std::string &chans, const std::string &keys = "");

    /* Leave channel(s) */
    void part(const std::string &chans);

    //void mode()

    /* Recieve topic, or set it, if a topic is passed. */
    void topic(const std::string &chan, const std::string &topic = "");

    /*
     * Recieve a list of nicknames on a channel, or a full list of
     * all channels and viewable nicknames, if one or more
     * channels aren't passed.
     */
    void names(const std::string &chans = "");

    /*
     * List all channels and their topics, or only list the topic
     * for the passed channel(s).
     */
    void list(const std::string &chans = "");

    /* Invite a nickname to a channel. */
    void invite(const std::string &nick, const std::string &chan);

    /* Kick nickname(s) from channel(s) with an optional comment. */
    void kick(const std::string &channel, const std::string &nick,
              const std::string &comment = "");

// Server queries and commands

    /* Query the version of the server program. */
    void version(const std::string &server = "");

    // Stats message

    // Links message

    // Time message

    // Connect message

    // Trace message

    // Admin message

    // Info message

// Sending messages

    /* Message a nick or a channel. */
    void msg(const std::string &target, const std::string &message);

    /* Notice a channel/nick with a message */
    void notice(const std::string &nick, const std::string &message);

// User-based queries

    // Who query

    // Whois query

    // Whowas query

// Miscellaneous messages

    // Kill message

    // Error message

// Optional messages

    /*
     * Unmark yourself as being away, or mark yourself as such,
     * if a 'message' is passed.
     */
    void away(const std::string &message = "");

// End of IRC commands.

    /*
     * As this lib is not feature-complete yet:
     * Send the given content directly to the server
     * as a command.
     */
    void raw_cmd(const std::string &content);

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
