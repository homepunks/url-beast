#include "common.hpp"
#include "handlers.hpp"
#include "server.hpp"

#include <exception>
#include <print>
#include <cstdio>

int main() {
  try {
    url_beast::boost_print_version();

    url_beast::Router router{};
    url_beast::register_routes(router);
    url_beast::run_server(router, url_beast::Port);
  } catch (const std::exception& e) {
    std::println(stderr, "fatal: {}", e.what());
    return -1;
  }
}
