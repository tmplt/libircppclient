#include <string>
#include <boost/tokenizer.hpp>

namespace gen {

typedef boost::tokenizer<boost::escaped_list_separator<char>> tokens_t;

bool is_integer(const std::string &s);

/* Hostname, ipv4, or ipv6. */
bool valid_addr(const std::string &url);

/* Split a string into multiple parts. */
tokens_t split_string(const std::string &s, const std::string &c);

}
