//
// Created by tom on 9/30/19.
//

#ifndef BABEL_THREAD_HPP
#define BABEL_THREAD_HPP


#include <memory>
#include <thread>
#include <mutex>
#include "shared/headers.hpp"

template <typename T>
class Thread {
public:
    explicit Thread(T *that);
    ~Thread();
    explicit operator bool();
    std::mutex & lock();

private:
    auto join() -> void;
    bool loop = true;
    bool isThreadRunning = false;
    std::shared_ptr<std::thread> thread;
	std::mutex _lock;
};

template<typename T>
Thread<T>::Thread(T *that) {
    this->thread = std::make_shared<std::thread>([that]() -> void {that->entryPoint();});
	if (!isThreadRunning)
		this->isThreadRunning = true;
	else
		this->join();
}

template<typename T>
Thread<T>::~Thread() {
    this->loop = false;
    this->join();
}

template<typename T>
Thread<T>::operator bool() {
    return loop;
}

template<typename T>
inline auto Thread<T>::lock() -> std::mutex &
{
	return this->_lock;
}

template<typename T>
inline auto Thread<T>::join() -> void
{
	thread->join();
}


#endif //BABEL_THREAD_HPP
