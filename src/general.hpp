#include <string>

namespace gen {

bool is_integer(const std::string &s);

/* domain, hostname, ipv4, or ipv6 */
bool valid_addr(const std::string &url);

}
