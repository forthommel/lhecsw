#ifndef Common_StringUtils_h
#define Common_StringUtils_h

#include <string>
#include <vector>

namespace utils {
  std::string ltrim(const std::string& str);                                     ///< Trim leading spaces
  std::string rtrim(const std::string& str);                                     ///< Trim trailing spaces
  inline std::string trim(const std::string& str) { return ltrim(rtrim(str)); }  ///< Trim leading and trailing spaces

  /// Split a string according to a separation character
  std::vector<std::string> split(const std::string&, char, bool trim_str = false);
}  // namespace utils

#endif
