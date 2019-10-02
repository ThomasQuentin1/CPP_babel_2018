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
    int isThreadRunning = 0;
    std::shared_ptr<std::thread> inThread;
	std::shared_ptr<std::thread> outThread;

	std::mutex _lock;
};

template<typename T>
Thread<T>::Thread(T *that) {
    this->inThread = std::make_shared<std::thread>([that]() -> void {that->readEntryPoint();});
	this->outThread = std::make_shared<std::thread>([that]() -> void {that->writeEntryPoint(); });

	if (isThreadRunning != 2)
		this->isThreadRunning++;
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
	inThread->join();
	outThread->join();
}


#endif //BABEL_THREAD_HPP
