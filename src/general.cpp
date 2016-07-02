#include <algorithm>
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include "general.hpp"

enum { addr_max_length = 255 };
const char hyphen = '-';

bool gen::is_integer(const std::string &s)
{
    for (char c: s) {
        if (!std::isdigit(c))
            return false;
    }

    return true;
}

gen::tokens_t gen::split_string(const std::string &str, const std::string &ch)
{
    gen::tokens_t tokens;
    boost::split(tokens, str, boost::is_any_of(ch));

    return tokens;
}

/* TODO: Implement support for internationalized domain names. */
std::string gen::valid_addr(const std::string &addr)
{
    using std::string;
    boost::system::error_code error;

    /*
     * Only checks if addr is in an ipv4/6 address.
     * Addresses such as irc.domain.tld and localhost
     * are invalid here.
     */
    boost::asio::ip::address::from_string(addr, error);

    if (error) {

        /*
         * As per RFC 1035:
         * https://blogs.msdn.microsoft.com/oldnewthing/20120412-00/?p=7873/
         * '.' are counted, also.
         */
        if (addr.length() > addr_max_length)
            return "the address is too long; it's illegal to exceed 255 characters.";

        /*
         * Split the hostname into its multiple sub-domains
         * (seperated by periods) and check them.
         */
        tokens_t tokens = split_string(addr, ".");

        for (auto &s: tokens) {

            /*
             * In case of "irc..hostname.tld", where the token between '..',
             * would be empty.
             */
            if (s.empty())
                return "a token is empty; does the address contain \"..\" somewhere?";

            /* Also as per RFC 1035. */
            if (s.front() == hyphen || s.back() ==  hyphen) {
                return "first or last character in the element \"" + s + '\"'
                     + " is a hyphen; that's not allowed.";
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

                return "the element \"" + s + '\"'
                     + " contains an illegal character (not a [A-Za-z0-9\\-]).";
            }
        }
    }

    return "";
}

