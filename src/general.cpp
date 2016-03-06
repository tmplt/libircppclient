#include <algorithm>
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/address.hpp>
#include <iostream>
#include "general.hpp"

using std::cout; using std::endl;

bool gen::is_integer(const std::string &s)
{
    /*
     * Credit to Charles Salvia;
     * http://stackoverflow.com/a/4654718
     *
     * Searches for the first element which does not
     * return false. If none are found, s.end() == s.end()
     * is expressed.
     */
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

bool gen::valid_addr(const std::string &addr)
{
    /* A hostname may not only consist of integers. */
    if (is_integer(addr))
        return false;

    /*
     * Only checks if addr is in an ipv4/6 address.
     * Addresses such as irc.dragons.rocks and localhost
     * are invalid here.
     */
    boost::system::error_code ec;
    boost::asio::ip::address::from_string(addr, ec);

    if (ec) {

        /* Do what the preview statement could not. */
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
