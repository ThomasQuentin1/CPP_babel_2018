//
// Created by tom on 9/21/19.
//

#ifndef BABEL_IPOLLABLE_HPP
#define BABEL_IPOLLABLE_HPP

#include "shared/headers.hpp"
#include "shared/types.hpp"
#include "IConnection.hpp"

namespace network {

    class IPollable {
    public:
        auto isReady() -> bool;
        auto preparePoll(struct pollfd &pfd);
    protected:
        IPollable() = default;
        sock s = 0;
    };

}


#endif //BABEL_IPOLLABLE_HPP
