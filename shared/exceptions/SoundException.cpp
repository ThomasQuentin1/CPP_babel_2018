//
// Created by tom on 9/18/19.
//

#include "SoundException.hpp"

ex::SoundException::SoundException(const std::string &what, const std::string &where) : _what(what), _where(where) {

}

auto ex::SoundException::what() -> std::string const & {
    return _what;
}

auto ex::SoundException::where() -> std::string const & {
    return _where;
}