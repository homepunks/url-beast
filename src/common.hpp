#pragma once

#include <boost/beast/http.hpp>
#include <boost/version.hpp>

#include <print>
#include <string>
#include <cstdint>
#include <functional>
#include <utility>
#include <random>

namespace url_beast {

namespace http = boost::beast::http;

inline constexpr std::uint16_t Port{6767};

using Request = http::request<http::string_body>;
using Response = http::response<http::string_body>;
using Handler = std::function<Response(const Request&)>;

inline void boost_print_version() {
  constexpr auto major = BOOST_VERSION / 100'000;
  constexpr auto minor = BOOST_VERSION / 100 % 1000;
  std::println("[Beast OK, Boost {}.{}]", major, minor);
}

inline Response json_response(const Request& req, http::status status, std::string body) {
  Response res{status, req.version()};
  res.set(http::field::content_type, "application/json");
  res.keep_alive(req.keep_alive());
  res.body() = std::move(body);
  res.prepare_payload();

  return res;
}

inline std::string generate_new_link() {
  thread_local std::mt19937 engine{std::random_device{}()};
  constexpr std::string_view alphabet{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};
  std::uniform_int_distribution<std::size_t> dist{0, alphabet.size() - 1};

  std::string result(6, '\0');
  for (char& c : result) c = alphabet[dist(engine)];
  return result;  
}

}
