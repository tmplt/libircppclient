#include <string>
#include <vector>

namespace gen {

bool is_integer(const std::string &s);

/*
 * Validates the syntax of hostnames, ipv4-, and ipv6-addresses.
 * Returns an empty string on success, non-empty on failure.
 */
std::string valid_addr(const std::string &addr);

/* Split a string into multiple strings when a character is met.
 * Returns all tokens in an array.
 */
using tokens_t = std::vector<std::string>;
tokens_t split_string(const std::string &str, const std::string &ch);

/* ns gen */
}

