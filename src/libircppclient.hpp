#pragma once
#include "connection.hpp"
#include "config.hpp"
#include <vector>

namespace irc {

class client {
public:
    client(const config &c);

    /* Check if config date is valid, if not return non-empty. */
    std::string validate_conf(const config &c);

    void start();
    void stop();

    /* Add a read_handler that may exist whereever. */
    void add_read_handler(read_handler_t func);

    /*
     * In all functions where a channel name must/may be passed,
     * the channel prefix [#|&|...] must be included.
     */

// RFC commands
// Connection registration:

    /*
     * Command: PASS
     * Sends password to the server?
     * Required before any registration message
     */

    /* Set nickname */
    void nick(const std::string &nick);

    /*
     * Command: USER
     * Required?
     */

    /* Get operator (+o) status */
    void oper(const std::string &nick, const std::string &pass);

    /*
     * Disconnect from the server with an optional quit message.
     * Also stops the irc::client.
     */
    void quit(const std::string &message = "");

// Channel operations:

    /* Join channel(s). */
    void join(const std::string &chans, const std::string &keys = "");

    /* Leave channel(s). */
    void part(const std::string &chans);

    /*
     * Set channel or user mode.
     * id is used because the argument does not have to be a nick;
     * it may be the full identifier, also.
     */
    void mode(const std::string &chan, const std::string &modes, const std::string &id = "");

    /* Recieve the topic, or set it, if a topic is passed. */
    void topic(const std::string &chan, const std::string &topic = "");

    /*
     * Recieve a list of nicknames on a channel, or a full list of
     * all channels and viewable nicknames, if no channel(s) is passed.
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
    void kick(const std::string &chan, const std::string &id, const std::string &comment = "");

// Sending messages:

    /* Message a nick or a channel. */
    void msg(const std::string &target, const std::string &message);

    /* Notice a channel/nick with a message */
    void notice(const std::string &target, const std::string &message);

// User-based queries:

    /*
     * Query info about all users, of users matching the search query.
     * If operators is true, query only for those.
     */
    void who(const std::string &query = "", const bool operators = false);

    /* Query info about particular users. */
    void whois(const std::string &ids, const std::string &server = "");

    /* Query info about a user that no longer exists. */
    void whowas(const std::string &nick, const std::string &count = "", const std::string &server = "");

// Optional commands:

    /*
     * Unmark yourself as being away, or mark yourself as such,
     * if a message is passed.
     */
    void away(const std::string &message = "");

// End of RFC commands.

    /*
     * Send the given content directly to the server
     * as a command.
     */
    void raw_cmd(const std::experimental::string_view &content);

private:
    config conf_;

    /*
     * Filled with lambdas at run-time which will decide how
     * to handle certain messages from the server.
     */
    void read_handler(const std::experimental::string_view &content);
    std::vector<read_handler_t> read_handlers_;

    connection con_;

    void initialize();
};

/* ns libircppclient */
}

