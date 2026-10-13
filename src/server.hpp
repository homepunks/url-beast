#pragma once

#include "router.hpp"
#include <cstdint>

namespace url_beast {
  
void run_server(const Router& router, std::uint16_t port);

}
