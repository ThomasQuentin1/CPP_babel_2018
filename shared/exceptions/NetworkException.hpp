//
// Created by tom on 9/18/19.
//

#ifndef BABEL_SOUNDEXCEPTION_HPP
#define BABEL_SOUNDEXCEPTION_HPP

#include <string>

namespace ex {

class NetworkException : public std::exception {
    public:
        explicit NetworkException(std::string const &what = "", std::string const &where = "");
        auto what() -> std::string const &;
        auto where() -> std::string const &;

    private:
        std::string _what;
        std::string _where;
    };
}


#endif //BABEL_SOUNDEXCEPTION_HPP
