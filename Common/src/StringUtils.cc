#include <algorithm>
#include <sstream>

#include "Common/include/StringUtils.h"

namespace utils {
  std::string ltrim(const std::string& str) {
    auto out{str};
    out.erase(out.begin(), std::find_if(out.begin(), out.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    return out;
  }

  std::string rtrim(const std::string& str) {
    auto out{str};
    out.erase(std::find_if(out.rbegin(), out.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(),
              out.end());
    return out;
  }

  std::vector<std::string> split(const std::string& str, char delim, bool trim_str) {
    std::vector<std::string> out;
    if (str.empty())
      return out;
    std::string token;
    std::istringstream iss(str);
    while (std::getline(iss, token, delim)) {
      const auto tok = trim_str ? trim(token) : token;
      if (!trim_str || !tok.empty())
        out.emplace_back(tok);
    }
    return out;
  }
}  // namespace utils
