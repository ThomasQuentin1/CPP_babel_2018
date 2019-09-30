/*
** EPITECH PROJECT, 2018
** Project
** File description:
** Class file
*/

#ifndef HEADERS_HPP
#define HEADERS_HPP

	// Commons C++ headers
	#include <memory>
	#include <string>
	#include <iostream>
	#include <stdexcept>
	#include <algorithm>
	#include <iomanip>
	#include <fstream>
	#include <csignal>
	#include <sstream>
    #include <deque>

	// Commons C headers
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>


	// Windows only
	#ifdef _WIN32
	#include <WinSock2.h>
	#include <Windows.h>
	#include <Ws2tcpip.h>
	#endif

	// Linux only
	#ifdef __linux__
        #include <sys/poll.h>
        #include <unistd.h>
        #define closesocket(socket) close(socket);
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#endif

	#ifdef _WIN32 // Windows types
	#define MSG_NOSIGNAL 0
	typedef int socklen_t;
	#endif


#endif
