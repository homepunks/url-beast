#pragma once

#include "common.hpp"

#include <utility>
#include <string>
#include <map>

namespace url_beast {

class Router {
  using Key = std::pair<http::verb, std::string>;
  std::map<Key, Handler> routes_{};

public:
  void add(http::verb method, std::string path, Handler handler);
  Response dispatch(const Request& req) const;
};

}
