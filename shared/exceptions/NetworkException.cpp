//
// Created by tom on 9/18/19.
//

#include "NetworkException.hpp"

ex::NetworkException::NetworkException(const std::string &what, const std::string &where) : _what(what), _where(where) {

}

auto ex::NetworkException::what() -> std::string const & {
    return _what;
}

auto ex::NetworkException::where() -> std::string const & {
    return _where;
}