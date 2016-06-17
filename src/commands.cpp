#include "libircppclient.hpp"

/*
 * Should these functions return something instead of
 * throwing stuff about?
 */

/*
 * Some IRC commands may have optional arguments.
 * These arguments, in this library, are the tail arguments,
 * even if they may not be that in the RFC.
 */

namespace irc {

void client::nick(const std::string &nick)
{
    if (nick.empty())
        throw std::invalid_argument("nick is empty.");

    con_.write("NICK " + nick);
}

void client::join(const std::string &chans, const std::string &keys)
{
    if (chans.empty())
        throw std::invalid_argument("chans is empty.");

    if (keys.empty())
        con_.write("JOIN " + chans);
    else
        con_.write("JOIN " + chans + ' ' + keys);
}

void client::msg(const std::string &target, const std::string &message)
{
    if (message.empty() || target.empty())
        throw std::invalid_argument("message or target empty");

    /* Applies to channels and nicks, both. */
    con_.write("PRIVMSG " + target + " :" + message);
}

void client::notice(const std::string &target, const std::string &message)
{
    if (message.empty() || target.empty())
        throw std::invalid_argument("target or message empty");

    con_.write("NOTICE " + target + " :" + message);
}

void client::who(const std::string &query, const bool operators)
{
    /* As per the RFC. */
    if (query.empty() || query == "0") {
        if (operators)
            con_.write("WHO o");
        else
            con_.write("WHO");
    }

    else {
        if (operators)
            con_.write("WHO " + query + " o");
        else
            con_.write("WHO " + query);
    }

}

void client::whois(const std::string &ids, const std::string &server)
{
    if (ids.empty())
        throw std::invalid_argument("ids is empty.");

    if (server.empty())
        con_.write("WHOIS " + ids);
    else
        con_.write("WHOIS " + server + ' ' + ids);
}

void client::whowas(const std::string &nick, const std::string &count, const std::string &server)
{
    if (nick.empty())
        throw std::invalid_argument("nick is empty");

    if (count.empty() && server.empty())
        con_.write("WHOWAS " + nick);

    else {
        if (server.empty())
            con_.write("WHOWAS " + nick + ' ' + count);
        else
            con_.write("WHOWAS " + nick + ' ' + count + ' ' + server);
    }
}

void client::oper(const std::string &nick, const std::string &pass)
{
    if (nick.empty() || pass.empty())
        throw std::invalid_argument("nick or pass empty");
    else
        con_.write("OPER " + nick + ' ' + pass);
}

void client::quit(const std::string &message)
{
    if (message.empty())
        con_.write("QUIT");
    else
        con_.write("QUIT :" + message);

    /* Currently broken. */
    stop();
}

void client::away(const std::string &message)
{
    if (message.empty())
        con_.write("AWAY");
    else
        con_.write("AWAY " + message);
}

void client::part(const std::string &chans)
{
    if (chans.empty())
        throw std::invalid_argument("chans is empty");

    con_.write("PART " + chans);
}

void client::mode(const std::string &chan, const std::string &modes, const std::string &id)
{
    if (chan.empty() || modes.empty())
        throw std::invalid_argument("chan or modes empty");

    if (id.empty())
        con_.write("MODE " + chan + ' ' + modes);
    else
        con_.write("MODE " + chan + ' ' + modes + ' ' + id);
}

void client::topic(const std::string &chan, const std::string &topic)
{
    if (chan.empty())
        throw std::invalid_argument("chan is empty");

    if (topic.empty())
        con_.write("TOPIC " + chan);
    else
        con_.write("TOPIC " + chan + " :" + topic);
}

void client::names(const std::string &chans)
{
    if (chans.empty())
        con_.write("NAMES");
    else
        con_.write("NAMES " + chans);
}

void client::invite(const std::string &nick, const std::string &chan)
{
    if (nick.empty() || chan.empty())
        throw std::invalid_argument("");

    con_.write("INVITE " + nick + ' ' + chan);
}

void client::kick(const std::string &chan, const std::string &id, const std::string &comment)
{
    if (chan.empty() || id.empty())
        throw std::invalid_argument("chan or id empty.");

    if (comment.empty())
        con_.write("KICK " + chan + ' ' + id);
    else
        con_.write("KICK " + chan + ' ' + id + " :" + comment);
}

void client::list(const std::string &chans)
{
    if (chans.empty())
        con_.write("LIST");
    else
        con_.write("LIST " + chans);
}

/* ns irc */
}
