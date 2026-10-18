#include "handlers.hpp"
#include "models.hpp"
#include "store.hpp"

#include <format>

namespace url_beast {

Response health(const Request& req) {
  return json_response(req, http::status::ok, R"({"status":"ok"})");
}

Response post_link(const Request& req, Link_Store& store) {
  auto payload_opt = deserialize_request_body(req.body());
  if (!payload_opt) return json_response(req, http::status::bad_request, R"({"error":"invalid request body"})");

  auto new_link = store.add(std::move(payload_opt->link));
  return json_response(req, http::status::created, 
      std::format(R"({{"status":"ok", "new_link":"{}"}})", new_link));
}

void register_routes(Router& router, Link_Store& store) {
  router.add(http::verb::get, "/health", health);
  router.add(http::verb::post, "/links", 
      [&store](const Request& req) { return post_link(req, store); });
}

}
