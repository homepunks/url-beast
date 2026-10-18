#pragma once

#include <string>
#include <string_view>
#include <optional>

namespace url_beast {

struct Link_Request {
  std::string link;
};

std::optional<Link_Request> deserialize_request_body(std::string_view body);

}
