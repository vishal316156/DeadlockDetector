#pragma once

#include <mutex>
#include <string>
#include <atomic>
#include "DeadlockRegistry.hpp"

class Mutex {
private:
    int id;
    std::string name;
    std::mutex nativeMutex;

    static int getNextId();

public:
    explicit Mutex(const std::string& name = "");
    ~Mutex() = default;

    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;

    void lock();
    void unlock();
    bool try_lock();

    int getId() const;
    const std::string& getName() const;
};

class LockGuard {
private:
    Mutex& mtx;

public:
    explicit LockGuard(Mutex& m);
    ~LockGuard();

    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};
