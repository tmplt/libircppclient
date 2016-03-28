#include <algorithm>
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/tokenizer.hpp>
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

gen::tokens_t gen::split_string(const std::string &s, const std::string &c)
{
    using boost::escaped_list_separator;
    using gen::tokens_t;
    using std::vector;
    using std::string;

    /* No escape character, nor quote character is used. */
    escaped_list_separator<char> delim("", c, "");
    tokens_t tokens(s, delim);

    return tokens;
}

bool gen::valid_addr(const std::string &addr)
{
    /*
     * Only checks if addr is in an ipv4/6 address.
     * Addresses such as irc.domain.tld and localhost
     * are invalid here.
     */
    boost::system::error_code ec;
    boost::asio::ip::address::from_string(addr, ec);

    /*
     * As per RFC 1035:
     * https://blogs.msdn.microsoft.com/oldnewthing/20120412-00/?p=7873/
     * '.' are counted, also.
     */
    if (addr.length() > 255) {
        cout << "[p] address length" << endl;
        return false;
    }

    if (ec) {

        /* Check if all domains in the hostname are valid. */
        tokens_t tokens = split_string(addr, ".");

        for (auto &s: tokens) {
            cout << s << endl;
        }

        cout << endl;
        exit(0);

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
