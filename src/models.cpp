#include <boost/json.hpp>
#include <boost/system/error_code.hpp>

#include "models.hpp"

#include <string>
#include <string_view>
#include <optional>

namespace url_beast {

std::optional<Link_Request> deserialize_request_body(std::string_view body) {
  boost::system::error_code ec{}; 
  auto jv = boost::json::parse(body, ec);
  if (ec) return std::nullopt;

  auto* obj = jv.if_object();
  if (!obj) return std::nullopt;
  auto* link_val = obj->if_contains("link");
  if (!link_val) return std::nullopt;

  auto* link_str = link_val->if_string();
  if (!link_str || link_str->empty()) return std::nullopt;
  return Link_Request{std::string{*link_str}};
}

}
