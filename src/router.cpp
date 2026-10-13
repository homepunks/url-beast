#include "router.hpp"

#include <string>
#include <exception>
#include <utility>

namespace url_beast {

void Router::add(http::verb method, std::string path, Handler handler) {
  routes_.emplace(Key{method, std::move(path)}, std::move(handler));
}

Response Router::dispatch(const Request& req) const {
  std::string path(req.target().data(), req.target().size());
  if (auto q{path.find('?')}; q != std::string::npos) path.resize(q);

  auto it{routes_.find(Key{req.method(), path})};
  if (it == routes_.end()) return json_response(req, http::status::not_found, R"({"error":"not found"})"); 

  try {
    return it->second(req);
  } catch (const std::exception&) {
    return json_response(req, http::status::internal_server_error, R"({"error":"internal error"})");
  }
}

}
