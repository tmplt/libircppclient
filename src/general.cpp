#include "general.hpp"
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

enum {
    /*
     * As per RFC 1035:
     * https://blogs.msdn.microsoft.com/oldnewthing/20120412-00/?p=7873/
     * '.' are counted, also.
     */
    addr_max_length = 255,

    hyphen   = '-',
    period   = '.',
    ipv6_sep = ':'
};

bool gen::is_integer(const std::experimental::string_view &s)
{
    for (char c: s) {
        if (!std::isdigit(c))
            return false;
    }

    return true;
}

bool gen::string_contains(const std::experimental::string_view &str, const char c)
{
    return str.find(c) != std::experimental::string_view::npos;
}

std::vector<std::string> gen::split_string(const std::experimental::string_view &str,
                                           const char c)
{
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

bool gen::valid_ipv46_addr(const std::experimental::string_view &addr)
{
    /* Unused, but required by inet_pton(). */
    unsigned char buf[sizeof(struct in6_addr)];

    if (string_contains(addr, period))
        return inet_pton(AF_INET, addr.data(), buf);
    else
        return inet_pton(AF_INET6, addr.data(), buf);
}

/* TODO: Implement support for internationalized domain names. */
const std::string gen::valid_addr(const std::experimental::string_view &addr)
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

        for (const std::string &s: tokens) {

            /*
             * In case of "irc..hostname.tld", where the token between '..',
             * would be empty.
             */
            if (s.empty())
                return "a token is empty; does the address contain \"..\" somewhere?";

            /* Also as per RFC 1035. */
            if (s.front() == hyphen || s.back() ==  hyphen) {
                return "first or last character in the element \""
                       + s + "\" is a hyphen; that's not allowed.";
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

                return "the element \"" + s
                       + "\" contains an illegal character (not a [A-Za-z0-9\\-]).";
            }
        }
    }

    return "";
}

