//
// Created by tom on 9/21/19.
//

#ifndef BABEL_ICONNECTION_HPP
#define BABEL_ICONNECTION_HPP


#include <memory>
#include <shared/types.hpp>

class IConnection {
public:
    // Tempalte
    template<typename T>
    auto send(T &data) -> int {
        return sendData(reinterpret_cast<bytes>(&data), sizeof(T));
    }

    template<typename T>
    auto recv() -> std::unique_ptr<T> {
        return std::unique_ptr<T>(reinterpret_cast<T*>(recvData(sizeof(T))));
    }

protected:
    IConnection() = default;
    virtual auto sendData(bytes buffer, int size) -> int = 0;
    virtual auto recvData(int size) -> bytes = 0;
};


#endif //BABEL_ICONNECTION_HPP
