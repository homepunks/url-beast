#include "handlers.hpp"
#include <print>

int main() {
  url_beast::Router router{};
  url_beast::register_routes(router);

  auto show = [&](url_beast::http::verb m, const char* target) {
    url_beast::Request req{m, target, 11};
    auto res = router.dispatch(req);
    std::println("{} {} -> {} {}", url_beast::http::to_string(m), target, res.result_int(), res.body());
  };

  show(url_beast::http::verb::get, "/health");
  show(url_beast::http::verb::get, "/nope");
}
