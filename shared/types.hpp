//
// Created by tom on 9/17/19.
//

#ifndef BABEL_TYPES_HPP
#define BABEL_TYPES_HPP

typedef unsigned char byte;
typedef unsigned char * bytes;

#ifdef _WIN32
typedef unsigned long long sock;
#endif
#ifdef __linux__ 
typedef int sock;
#endif

#endif //BABEL_TYPES_HPP
