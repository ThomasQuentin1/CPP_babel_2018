//
// Created by tom on 9/21/19.
//

#include <shared/exceptions/NetworkException.hpp>
#include "Timeout.hpp"
#include "shared/headers.hpp"

Timeout::Timeout(int _timeout) : timeout(_timeout) {
    this->last = time(nullptr);
}

auto Timeout::check() -> void {
    if (time(nullptr) - this->last > 10)
        throw ex::NetworkException("Connection timed out", "timeout check");
}

auto Timeout::refresh() -> void {
    this->last = time(nullptr);
}
