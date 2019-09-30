//
// Created by tom on 9/21/19.
//

#include "shared/headers.hpp"
#include "IPollable.hpp"
#include <shared/exceptions/NetworkException.hpp>

auto network::IPollable::isReady() -> bool {

	struct pollfd fds[1];
    int ret;

    fds[0].fd = this->s;
    fds[0].events = POLLIN;

#ifdef _WIN32
	ret = WSAPoll(fds, 1, 0);
#endif
#ifdef __linux__ 
	ret = poll(fds, 1, 0);
#endif

    if (ret == -1)
        throw ex::NetworkException("unable to poll", "isReady connection");
    return (fds[0].revents & POLLIN);
}

auto network::IPollable::preparePoll(struct pollfd &pfd) {
    pfd.events = POLLIN;
    pfd.fd = this->s;
}
