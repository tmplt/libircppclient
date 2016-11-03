#include <cassert>
#include <algorithm>
#include <arpa/inet.h>
#include "util.hpp"

enum {
    /*
     * As per RFC 1035:
     * https://blogs.msdn.microsoft.com/oldnewthing/20120412-00/?p=7873/
     * '.' are counted, also.
     */
    addr_max_length = 255,
};

enum {
    ipv4,
    ipv6
};

enum {
    hyphen   = '-',
    period   = '.',
    ipv6_sep = ':'
};

bool util::is_integer(const std::experimental::string_view &s)
{
    assert(!s.empty());

    for (char c: s) {
        if (!std::isdigit(c))
            return false;
    }

    return true;
}

bool util::string_contains(const std::experimental::string_view &str, const char c)
{
    assert(!str.empty());
    assert(c);

    return str.find(c) != std::experimental::string_view::npos;
}

int util::ip_version(const std::experimental::string_view &addr)
{
    assert(!addr.empty());

    /*
     * Feels hackish.
     *
     * TODO: Look into and confirm/bust above statement.
     */
    if (string_contains(addr, period))
        return ipv4;

    return ipv6;
}

/*
 * Split a string into multiple strings when a character is met.
 * Returns all tokens in an array.
 */
std::vector<std::string> util::split_string(const std::experimental::string_view &str, const char c)
{
    assert(!str.empty());
    assert(c);

    using std::experimental::string_view;
    std::vector<std::string> result;

    for (string_view::const_iterator len = str.begin(); len <= str.end(); len++) {
        string_view::const_iterator token_start = len;

        while (*len != c && *len)
            len++;

        result.emplace_back(token_start, len);
    }

    return result;
}

/*
 * Validates an ipv4ipv6 address and return the result.
 * Thanks, beej!
 */
bool util::valid_ipv46_addr(const std::experimental::string_view &addr)
{
    assert(!addr.empty());

    /* Unused, but required by inet_pton(). */
    unsigned char buf[sizeof(struct in6_addr)];

    if (ip_version(addr) == ipv4)
        return inet_pton(AF_INET, addr.data(), buf);
    else
        return inet_pton(AF_INET6, addr.data(), buf);
}

/*
 * Validates the syntax of hostnames, ipv4-, and ipv6-addresses.
 * Returns an empty string on success, non-empty on failure.
 *
 * TODO: Implement support for internationalized domain names.
 */
const std::string util::valid_addr(const std::experimental::string_view &addr)
{
    if (!valid_ipv46_addr(addr)) {

        if (string_contains(addr, ipv6_sep))
            return "invalid ipv6 address.";

        if (addr.length() > addr_max_length)
            return "the address is too long; it's illegal to exceed 255 characters.";

        /*
         * Split the hostname into its multiple sub-domains
         * (seperated by periods) and check them.
         */
        std::vector<std::string> tokens = split_string(addr, period);

        for (const std::experimental::string_view &s: tokens) {

            /*
             * In case of "irc..hostname.tld", where the token between '..',
             * would be empty.
             */
            if (s.empty())
                return "a token is empty; does the address contain \"..\" somewhere?";

            /* Also as per RFC 1035. */
            if (s.front() == hyphen || s.back() ==  hyphen) {
                std::string element = s.data();
                return "first or last character in the element \"" +
                       element + "\" is a hyphen; that's not allowed.";
            }

            /*
             * O̶n̶l̶y̶ ̶[̶A̶-̶Z̶a̶-̶z̶0̶-̶9̶]̶ ̶a̶r̶e̶ ̶a̶l̶l̶o̶w̶e̶d̶.̶
             * Okey, no. International domain names is a thing:
             * <https://en.wikipedia.org/wiki/Domain_name>
             *
             * s[first] and s[last] are checked again, but they
             * must be checked for non-[A-Za-z0-9], anyway. A check for
             * periods could be implemented here, but then we'd lose a specified
             * error message and we'd need more checks than what we remove.
             */
            if (std::find_if(s.begin(), s.end(),
                [](char c) {
                    return !std::isdigit(c) &&
                           !std::isalpha(c) &&
                           c != hyphen;     }) != s.end()) {

                std::string element = s.data();
                return "the element \"" + element +
                       "\" contains an illegal character (not a [A-Za-z0-9\\-]).";
            }
        }
    }

    return "";
}

