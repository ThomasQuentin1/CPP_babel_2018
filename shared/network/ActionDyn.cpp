//
// Created by tom on 9/21/19.
//

#include <shared/exceptions/NetworkException.hpp>
#include "ActionDyn.hpp"

packet::ActionDyn::ActionDyn(packet::action const &raw_action, bytes raw_body) {
    if (raw_action.magic != 0xAC)
        throw ex::NetworkException("bad packet identified", "actionDyn constructor");

    this->_code = static_cast<packet::operation>(raw_action.action_code);
    if (raw_body)
        this->_body = reinterpret_cast<char*>(raw_body);
}

auto packet::ActionDyn::code() const -> packet::operation {
    return this->_code;
}

auto packet::ActionDyn::body() const -> std::string const & {
    return this->_body;
}
