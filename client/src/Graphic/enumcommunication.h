#ifndef ENUMCOMMUNICATION_H
#define ENUMCOMMUNICATION_H

typedef enum ReturnVal {
    NONE, //                                                        0
    INCOMMING_CALL, // This value is received by arg in refresh     1
    ACCEPT_CALL, // This value is send with a return in refresh     2
    DECLINE_CALL, // This value is send with a return in refresh    3
    CALL, // This value is send with a return in refresh            4
    IS_COMMUNICATING, // This value is received by arg in refresh   5
    CALL_ACCEPTED, // This value is received by arg in refresh      6
    CALL_DECLINED, // This value is received by arg in refresh      7
    END_CALL, // This value is received and send                    8
    TRY_LOGIN, // This value is send with a return in refresh       9
    TRY_REGISTER, // This value is send with a return in refresh    10
    CONNECTION_SUCCESS, // This value is received by arg in refresh 11
    CONNECTION_FAILED, // This value is received by arg in refresh   12
    LOGOUT,
} commEnum_t;

#endif // ENUMCOMMUNICATION_H
