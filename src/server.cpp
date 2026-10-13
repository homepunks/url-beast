#include "server.hpp"

#include <boost/asio/dispatch.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <chrono>
#include <cstddef>
#include <print>
#include <utility>
#include <memory>
#include <cstdio>

namespace url_beast {
  
namespace {
  
namespace beast = boost::beast;
namespace net = boost::asio;
using tcp = net::ip::tcp;

void log_error(beast::error_code ec, const char* what) {
  if (ec == net::error::operation_aborted || ec == http::error::end_of_stream) return;
  std::println(stderr, "{}: {}", what, ec.message());
}

class Session : public std::enable_shared_from_this<Session> {
  beast::tcp_stream stream_;
  beast::flat_buffer buf_;
  Request req_;
  Response res_;
  const Router& router_;
  
public:
  Session(tcp::socket&& sock, const Router& router)
    : stream_{std::move(sock)}, router_{router} {};

  void run() {
    net::dispatch(stream_.get_executor(),
        [self = shared_from_this()] { self->do_read(); });
  }

private:
  void do_read() {
    req_ = {};
    stream_.expires_after(std::chrono::seconds{10});
    http::async_read(stream_, buf_, req_,
        [self = shared_from_this()](beast::error_code ec, std::size_t) {
          self->on_read(ec);
        });
  }

  void on_read(beast::error_code ec) {
    if (ec == http::error::end_of_stream) return do_close(); 
    if (ec) return log_error(ec, "read");

    res_ = router_.dispatch(req_);
    http::async_write(stream_, res_,
        [self = shared_from_this()](beast::error_code ec, std::size_t) {
          self->on_write(ec);
        });
  }

  void on_write(beast::error_code ec) {
    if (ec) return log_error(ec, "write");
    if (res_.need_eof()) return do_close();
    do_read();
  }

  void do_close() {
    beast::error_code ec;
    stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
  }
};

class Listener : public std::enable_shared_from_this<Listener> {
  net::io_context& ioc_;
  tcp::acceptor acceptor_;
  const Router& router_;

public:
  Listener(net::io_context& ioc, tcp::endpoint ep, const Router& router)
      : ioc_{ioc}, acceptor_{ioc, ep}, router_{router} {}

  void run() { do_accept(); }

private:
  void do_accept() {
    acceptor_.async_accept(net::make_strand(ioc_),
        [self = shared_from_this()](beast::error_code ec, tcp::socket sock) {
          if (ec == net::error::operation_aborted) return;
          if (ec) log_error(ec, "accept");
          else std::make_shared<Session>(std::move(sock), self->router_)->run();
          self->do_accept();
        });
  }
};

}

void run_server(const Router& router, std::uint16_t port) {
  net::io_context ioc{1};
  std::make_shared<Listener>(ioc, tcp::endpoint{tcp::v4(), port}, router)->run();
  std::println("listening on port {}", port);
  ioc.run();
}

}
