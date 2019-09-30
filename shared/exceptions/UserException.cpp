//
// Created by tom on 9/18/19.
//

#include "UserException.hpp"

ex::UserException::UserException(const std::string &what, const std::string &where) : _what(what), _where(where) {}

auto ex::UserException::what() -> std::string const & {
    return _what;
}

auto ex::UserException::where() -> std::string const & {
    return _where;
}