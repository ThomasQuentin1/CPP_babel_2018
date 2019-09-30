//
// Created by tom on 9/30/19.
//

#ifndef BABEL_THREAD_HPP
#define BABEL_THREAD_HPP


#include <memory>
#include <thread>

template <typename T>
class Thread {
public:
    explicit Thread(T *that);
    ~Thread();
    explicit operator bool();

private:
    auto join() -> void;
    bool loop = true;
    bool isThreadRunning = false;
    std::unique_ptr<std::thread> thread;
};

template<typename T>
Thread<T>::Thread(T *that) {
    this->thread = std::make_unique<std::thread>([that]() -> void {that->entryPoint();});
	if (isThreadRunning)
		this->isThreadRunning = true;
}

template<typename T>
Thread<T>::~Thread() {
    this->loop = false;
    thread->join();
}

template<typename T>
Thread<T>::operator bool() {
    return loop;
}


#endif //BABEL_THREAD_HPP
