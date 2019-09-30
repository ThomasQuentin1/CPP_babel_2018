//
// Created by tom on 9/21/19.
//

#ifndef WINDOWS_INIT_HPP
#define WINDOWS_INIT_HPP

#include "shared/headers.hpp"

namespace network {

    class WindowsInit {
    public:
		WindowsInit();
		~WindowsInit();

	private:
#ifdef _WIN32
		WSADATA wsaData;
#endif
    };
}

#endif
