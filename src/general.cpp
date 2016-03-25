#include <algorithm>
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/address.hpp>
#include "general.hpp"

bool gen::is_integer(const std::string &s)
{
    for (char c: s) {
        if (!std::isdigit(c))
            return false;
    }

    return true;
}

bool gen::valid_addr(const std::string &addr)
{
    /* A hostname may not only consist of integers. */
    if (is_integer(addr))
        return false;

    /*
     * Only checks if addr is in an ipv4/6 address.
     * Addresses such as irc.domain.tld and localhost
     * are invalid here.
     */
    boost::system::error_code ec;
    boost::asio::ip::address::from_string(addr, ec);

    if (ec) {

        /* Do what the previous statement could not. */
        if (std::find_if(addr.begin(), addr.end(),
            [](char c) {
                return !std::isdigit(c) &&
                       !std::isalpha(c) &&
                       c != '-'         &&
                       c != '_'         &&
                       c != '.';        }) != addr.end())
            return false;
    }

    return true;
}
