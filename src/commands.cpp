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

namespace libircppclient {

void client::nick(const std::string &nick)
{
    if (nick.empty())
        return;

    con.write("NICK " + nick);
}

void client::join(const std::string &chan, const std::string &key)
{
    if (chan.empty())
        return;

    if (key.empty())
        con.write("JOIN " + chan);
    else
        con.write("JOIN " + chan + ' ' + key);
}

void client::msg(const std::string &target, const std::string &message)
{
    if (message.empty() || target.empty())
        return;

    /* Applies to channels and nicks */
    con.write("PRIVMSG " + target + " :" + message);
}

void client::quit(const std::string &message)
{
    if (message.empty())
        con.write("QUIT");
    else
        con.write("QUIT :" + message);

    stop();
}

/* ns libircppclient */
}
