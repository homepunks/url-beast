#include "common.hpp"
#include "handlers.hpp"
#include "server.hpp"
#include "store.hpp"

#include <exception>
#include <print>
#include <cstdio>

int main() {
  try {
    url_beast::boost_print_version();

    url_beast::Link_Store store{};
    url_beast::Router router{};
    url_beast::register_routes(router, store);
    url_beast::run_server(router, url_beast::Port);
  } catch (const std::exception& e) {
    std::println(stderr, "fatal: {}", e.what());
    return -1;
  }
}
