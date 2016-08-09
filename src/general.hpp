#include <string>
#include <vector>
#include <experimental/string_view>

namespace gen {

bool is_integer(const std::experimental::string_view &s);
bool string_contains(const std::experimental::string_view &str, const char c);

/*
 * Validates the syntax of hostnames, ipv4-, and ipv6-addresses.
 * Returns an empty string on success, non-empty on failure.
 */
const std::string valid_addr(const std::experimental::string_view &addr);
bool valid_ipv46_addr(const std::experimental::string_view &addr);

/* Split a string into multiple strings when a character is met.
 * Returns all tokens in an array.
 */
std::vector<std::string> split_string(const std::experimental::string_view &str, const char c);

/* ns gen */
}

