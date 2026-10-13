#include "handlers.hpp"

namespace url_beast {

Response health(const Request& req) {
  return json_response(req, http::status::ok, R"({"status":"ok"})");
}

void register_routes(Router& router) {
  router.add(http::verb::get, "/health", health);
}
}
