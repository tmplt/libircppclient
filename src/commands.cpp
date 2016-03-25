#include "libircppclient.hpp"

/*
 * TODO: These functions should return a value instead
 * of nothing.
 * Put returned errors in some kind of error stream?
 */

/*
 * While there shouldn't be a reason to not pass an
 * argument to a command that requires it, it may still happen.
 * Return errors for those exceptions?
 */

namespace irc {

void client::nick(const std::string &nick)
{
    if (nick.empty())
        throw std::invalid_argument("nick is empty.");

    con.write("NICK " + nick);
}

void client::join(const std::string &chans, const std::string &keys)
{
    if (chans.empty())
        throw std::invalid_argument("chans is empty.");

    if (keys.empty())
        con.write("JOIN " + chans);
    else
        con.write("JOIN " + chans + ' ' + keys);
}

void client::msg(const std::string &target, const std::string &message)
{
    if (message.empty() || target.empty())
        throw std::invalid_argument("message or target empty");

    /* Applies to channels and nicks, both */
    con.write("PRIVMSG " + target + " :" + message);
}

void client::notice(const std::string &target, const std::string &message)
{
    if (message.empty() || target.empty())
        throw std::invalid_argument("target or message empty");

    con.write("NOTICE " + target + " :" + message);
}

void client::who(const std::string &query, const bool operators)
{
    /* As per the RFC. */
    if (query.empty() || query == "0") {
        if (operators)
            con.write("WHO o");
        else
            con.write("WHO");
    } else {
        if (operators)
            con.write("WHO " + query + " o");
        else
            con.write("WHO " + query);
    }

}

void client::oper(const std::string &nick, const std::string &pass)
{
    if (nick.empty() || pass.empty())
        throw std::invalid_argument("nick or pass empty");

    else
        con.write("OPER " + nick + ' ' + pass);
}

void client::quit(const std::string &message)
{
    if (message.empty())
        con.write("QUIT");
    else
        con.write("QUIT :" + message);

    stop();
}

void client::away(const std::string &message)
{
    if (message.empty())
        con.write("AWAY");
    else
        con.write("AWAY " + message);
}

void client::part(const std::string &chans)
{
    if (chans.empty())
        throw std::invalid_argument("chans is empty");

    con.write("PART " + chans);
}

void client::mode(const std::string &chan, const std::string &modes, const std::string &id)
{
    if (chan.empty() || modes.empty())
        throw std::invalid_argument("chan or modes empty");

    if (id.empty())
        con.write("MODE " + chan + ' ' + modes);
    else
        con.write("MODE " + chan + ' ' + modes + ' ' + id);
}

void client::topic(const std::string &chan, const std::string &topic)
{
    if (chan.empty())
        throw std::invalid_argument("chan is empty");

    if (topic.empty())
        con.write("TOPIC " + chan);
    else
        con.write("TOPIC " + chan + " :" + topic);
}

void client::names(const std::string &chans)
{
    if (chans.empty())
        con.write("NAMES");
    else
        con.write("NAMES " + chans);
}

void client::invite(const std::string &nick, const std::string &chan)
{
    if (nick.empty() || chan.empty())
        throw std::invalid_argument("");

    con.write("INVITE " + nick + ' ' + chan);
}

void client::kick(const std::string &chan, const std::string &id, const std::string &comment)
{
    if (chan.empty() || id.empty())
        throw std::invalid_argument("chan or id empty.");

    if (comment.empty())
        con.write("KICK " + chan + ' ' + id);
    else
        con.write("KICK " + chan + ' ' + id + " :" + comment);
}

void client::list(const std::string &chans)
{
    if (chans.empty())
        con.write("LIST");
    else
        con.write("LIST " + chans);
}

/* ns irc */
}
