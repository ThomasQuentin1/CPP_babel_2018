//
// Created by tom on 9/18/19.
//

#ifndef BABEL_PACKETS_HPP
#define BABEL_PACKETS_HPP

#include "types.hpp"

namespace packet
{
    struct data // UDP
    {
        unsigned short magic = 0xDA;
        int date;
        short size;
        byte body[5024];
    };

    struct action // TCP
    {
        unsigned short magic = 0xAC;
        char action_code;
        short body_size;
        // Send body after this header
    };

    enum operation
    {
        OK,             // 0
        KO,             // 1
        LOGIN,          // 2
        REGISTER,       // 3
        DISCONNECT,     // 4
        CALL_START,     // 5
        CALL_END,       // 6
        INCOMMING_CALL, // 7
    };
}

#endif //BABEL_PACKETS_HPP
