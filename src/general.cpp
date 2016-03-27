#include <algorithm>
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/address.hpp>
#include <iostream>
#include "general.hpp"

using std::cout; using std::endl;

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
    /*
     * As per RFC 1035:
     * https://blogs.msdn.microsoft.com/oldnewthing/20120412-00/?p=7873/
     * '.' are counted, also.
     */
    if (addr.length() > 255) {
        cout << "[p] address length" << endl;
        return false;
    }

    /*
     * Only checks if addr is in an ipv4/6 address.
     * Addresses such as irc.domain.tld and localhost
     * are invalid here.
     */
    boost::system::error_code ec;
    boost::asio::ip::address::from_string(addr, ec);

    /*
     * If they exist, split the domain at '.',
     * and check every part.
     */

    if (ec) {
        cout << "[f] not a ipv4/6 address" << endl;

        /*
         * A domain may not start with a hyphen,
         * or any other special character.
         */
        if (!std::isalpha(addr[0]) && !std::isdigit(addr[0])) {
            //if (std::isalpha(addr[0]))
            //    cout << '\''<< addr[0] << "' is not a letter" << endl;
            cout << "[f] addr[0] invalid ('" << addr[0] << "')" << endl;
            return false;
        } else
            cout << "[p] addr[0] valid" << endl;

        /*
         * We have already checked the first character,
         * but anything past it may denote a nth-level domain,
         * or contain a hyphen.
         */
        if (std::find_if(addr.begin() + 1, addr.end(),
            [](char c) {
                cout << c;
                return !std::isdigit(c) &&
                       !std::isalpha(c) &&
                       c != '-'         &&
                       c != '.';        }) != addr.end())
        {
            cout << endl;
            return false;
        }
    }

    return true;
}
