#include "store.hpp"
#include "common.hpp"

#include <utility>
#include <optional>
#include <string>
#include <string_view>
#include <format>

namespace url_beast {

std::string Link_Store::add(std::string&& url) {
  for (;;) {
    auto new_link = std::format("127.0.0.1:6767/{}", generate_new_link());
    auto [_, inserted] = links_.try_emplace(new_link, std::move(url));
    if (inserted) return new_link;
  }
}

std::optional<std::string_view> Link_Store::find(const std::string& url) const {
  auto it = links_.find(url);
  if (it == links_.end()) return std::nullopt;
  return it->second;
}

}
