#pragma once

#include "common.hpp"
#include "router.hpp"
#include "store.hpp"

namespace url_beast {

Response health(const Request& req);

Response post_link(const Request& req, Link_Store& store);

void register_routes(Router& router, Link_Store& store);

}
