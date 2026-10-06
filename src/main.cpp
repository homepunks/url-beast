#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include "util.hpp"

#include <cstdlib>
#include <print>

namespace beast = boost::beast;
namespace http  = beast::http;
namespace net   = boost::asio;

inline constexpr int Port{6767};

void health_check(net::ip::tcp::socket& sock) {
  beast::flat_buffer buf{};
  http::request<http::string_body> req{};
  http::read(sock, buf, req);

  http::response<http::string_body> res{};
  res.version(req.version());
  res.set(http::field::content_type, "application/json");

  if (req.method() == http::verb::get && req.target() == "/health") {
    res.result(http::status::ok);
    res.body() = R"({"status": "ok"})";
  } else {
    res.result(http::status::not_found);
    res.body() = R"({"error":"not found"})";
  }
  res.keep_alive(false);
  res.prepare_payload();
  http::write(sock, res);
}

int main() {
  net::io_context ioc{};
  net::ip::tcp::acceptor acceptor{ioc, {net::ip::tcp::v4(), Port}};
  boost_print_version();
  std::println("listening on {}", Port);
  
  for (;;) {
    net::ip::tcp::socket sock{ioc};
    acceptor.accept(sock);
    try {
      health_check(sock);
    } catch (const std::exception& e) {
      std::println("error: {}", e.what());
    }
    sock.shutdown(net::ip::tcp::socket::shutdown_send);
  }
  return EXIT_SUCCESS;
}
