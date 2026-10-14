#include "handlers.hpp"

namespace url_beast {

Response health(const Request& req) {
  return json_response(req, http::status::ok, R"({"status":"ok"})");
}

Response post_link(const Request& req) {
  return json_response(req, http::status::created, R"({"status":"ok", "new_link":"127.0.0.1:6969/links/foo"})");
}

void register_routes(Router& router) {
  router.add(http::verb::get, "/health", health);
  router.add(http::verb::post, "/links", post_link);
}

}
