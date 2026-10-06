#pragma once
#include <boost/version.hpp>
#include <print>

inline void boost_print_version() noexcept {
  constexpr auto major = BOOST_VERSION / 100'000;
  constexpr auto minor = BOOST_VERSION / 100 % 1000;
  std::println("[Beast OK, Boost {}.{}]", major, minor);
}
