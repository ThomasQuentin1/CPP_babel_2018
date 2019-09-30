//
// Created by tom on 9/21/19.
//

#include "WindowsInit.hpp"
#include "shared/exceptions/NetworkException.hpp"

network::WindowsInit::WindowsInit() {
#ifdef _WIN32
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		throw ex::NetworkException("unable to start winsock", "windows init");
#endif
}

network::WindowsInit::~WindowsInit()
{
#ifdef _WIN32
	WSACleanup();
#endif
}
