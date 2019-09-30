//
// Created by tom on 9/21/19.
//

#ifndef BABEL_UDPPROXY_HPP
#define BABEL_UDPPROXY_HPP

#include <string>

namespace network {

    class UdpProxy {
    public:
        UdpProxy(std::string const &ip1, std::string const &ip2, short port1 = rand()%5000+1000, short port2 = rand()%5000+1000);
    };

}

#endif //BABEL_UDPPROXY_HPP
