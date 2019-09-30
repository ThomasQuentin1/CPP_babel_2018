//
// Created by tom on 9/18/19.
//

#include "IOException.hpp"

ex::IOException::IOException(const std::string &what, const std::string &where) : _what(what), _where(where) {

}

auto ex::IOException::what() -> std::string const & {
    return _what;
}

auto ex::IOException::where() -> std::string const & {
    return _where;
}