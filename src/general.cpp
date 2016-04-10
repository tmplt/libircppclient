#include <algorithm>
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include "general.hpp"

bool gen::is_integer(const std::string &s)
{
    for (char c: s) {
        if (!std::isdigit(c))
            return false;
    }

    return true;
}

gen::tokens_t gen::split_string(const std::string &str, const std::string &c)
{
    gen::tokens_t tokens;
    boost::split(tokens, str, boost::is_any_of(c));

    return tokens;
}

std::string gen::valid_addr(const std::string &addr)
{
    using std::string;;

    /*
     * Only checks if addr is in an ipv4/6 address.
     * Addresses such as irc.domain.tld and localhost
     * are invalid here.
     */
    boost::system::error_code ec;
    boost::asio::ip::address::from_string(addr, ec);

    if (ec) {

        /*
         * As per RFC 1035:
         * https://blogs.msdn.microsoft.com/oldnewthing/20120412-00/?p=7873/
         * '.' are counted, also.
         */
        if (addr.length() > 255)
            return "the address is too long; it's illegal to exceed 255 characters.";

        /*
         * Split the hostname into multiple domains
         * and check each element.
         */

        tokens_t tokens = split_string(addr, ".");

        for (auto &s: tokens) {

            /*
             * In case of "irc..hostname.tld", where the token between '..',
             * would be empty.
             */
            if (s.empty())
                return "a token is empty; does the address contain \"..\"?";

            /* Also as per RFC 1035. */
            if (s.front() == '-' || s.back() ==  '-') {
                return  "first or last character in the domain \"" + s + '\"'
                        + " is a hyphen; that's not allowed.";
            }

            /*
             * Only [A-Za-z0-9] are allowed.
             *
             * s[first] and s[last] are checked again, but they
             * must be checked for non-[A-Za-z0-9], anyway.
             */
            if (std::find_if(s.begin(), s.end(),
                [](char c) {
                    return !std::isdigit(c) &&
                           !std::isalpha(c) &&
                           c != '-';        }) != s.end()) {

                return "the domain \"" + s + '\"'
                       + " contains an illegal character (not a [A-Za-z0-9\\-]).";
            }
        }
    }

    return "";
}

