#include <string>
#include <vector>

namespace gen {

bool is_integer(const std::string &s);

/*
 * Hostname, ipv4, or ipv6.
 * Throws std::invalid_argument.
 */
void valid_addr(const std::string &url);

/* Split a string into count(c) parts, return all tokens in an array. */
typedef std::vector<std::string> tokens_t;
tokens_t split_string(const std::string &str, const std::string &c);

}
