#pragma once

#include "common.hpp"
#include "router.hpp"

namespace url_beast {

Response health(const Request& req);

Response post_link(const Request& req);

void register_routes(Router& router);

}
