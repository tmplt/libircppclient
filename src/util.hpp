#include <string>
#include <vector>
#include <experimental/string_view>

namespace util {

bool is_integer(const std::experimental::string_view &s);
bool string_contains(const std::experimental::string_view &str, const char c);
bool valid_ipv46_addr(const std::experimental::string_view &addr);
int ip_version(const std::experimental::string_view &addr);

const std::string valid_addr(const std::experimental::string_view &addr);
std::vector<std::string> split_string(const std::experimental::string_view &str, const char c);

/* ns util */
}

