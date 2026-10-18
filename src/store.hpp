#pragma once

#include <unordered_map>
#include <string>
#include <optional>
#include <string_view>

namespace url_beast {

class Link_Store {
  std::unordered_map<std::string, std::string> links_{};

public:
  std::string add(std::string&& url);
  std::optional<std::string_view> find(const std::string& url) const;
};

}
