//
// Created by tom on 9/21/19.
//

#ifndef BABEL_ACTIONDYN_HPP
#define BABEL_ACTIONDYN_HPP

#include <shared/packets.hpp>
#include <string>

namespace packet {

    class ActionDyn {
    public:
        explicit ActionDyn(packet::action const &raw_action, bytes raw_body = nullptr);
        auto code() const -> packet::operation;
        auto body() const -> std::string const &;
    private:
        packet::operation _code;
        std::string _body;
    };

}

#endif //BABEL_ACTIONDYN_HPP
