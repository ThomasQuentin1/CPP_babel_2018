//
// Created by tom on 9/21/19.
//

#ifndef BABEL_TIMEOUT_HPP
#define BABEL_TIMEOUT_HPP


class Timeout {
public:
    explicit Timeout(int _timeout = 10);
    Timeout(const Timeout &) = delete;
    Timeout(Timeout &&) = default;
    auto check() -> void;
    auto refresh() -> void;

private:
    int last;
    int timeout;
};


#endif //BABEL_TIMEOUT_HPP
