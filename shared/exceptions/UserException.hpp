//
// Created by tom on 9/18/19.
//

#ifndef BABEL_USEREXCEPTION_HPP
#define BABEL_USEREXCEPTION_HPP

#include "shared/headers.hpp"

namespace ex {

    class UserException : public std::exception {
        public:
            explicit UserException(std::string const &what = "", std::string const &where = "");
            auto what() -> std::string const &;
            auto where() -> std::string const &;

        private:
            std::string _what;
            std::string _where;
    };
}


#endif //BABEL_USEREXCEPTION_HPP
