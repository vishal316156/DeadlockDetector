#include "DeadlockDetector.hpp"

int Mutex::getNextId() {
    static std::atomic<int> counter{1};
    return counter.fetch_add(1);
}

Mutex::Mutex(const std::string& name) : id(getNextId()), name(name) {
    if (this->name.empty()) {
        this->name = "Lock_" + std::to_string(id);
    }
    DeadlockRegistry::instance().registerLock(id, this->name);
}

void Mutex::lock() {
    DeadlockRegistry::instance().beforeLock(id, name);
    nativeMutex.lock();
    DeadlockRegistry::instance().afterLock(id, name);
}

void Mutex::unlock() {
    DeadlockRegistry::instance().beforeUnlock(id, name);
    nativeMutex.unlock();
}

bool Mutex::try_lock() {
    DeadlockRegistry::instance().beforeLock(id, name);
    bool acquired = nativeMutex.try_lock();
    if (acquired) {
        DeadlockRegistry::instance().afterLock(id, name);
    }
    return acquired;
}

int Mutex::getId() const {
    return id;
}

const std::string& Mutex::getName() const {
    return name;
}

LockGuard::LockGuard(Mutex& m) : mtx(m) {
    mtx.lock();
}

LockGuard::~LockGuard() {
    mtx.unlock();
}
