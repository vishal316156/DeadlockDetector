#pragma once

#include <mutex>
#include <atomic>
#include <cstdint>
#include <string>
#include "LockGraph.hpp"
#include "ThreadState.hpp"
#include "Reporter.hpp"

enum class DeadlockPolicy {
    WARN_ONLY,
    THROW_EXCEPTION
};

class DeadlockRegistry {
private:
    LockGraph graph;
    Reporter reporter;
    std::mutex internalMutex;
    std::atomic<int> deadlockDetectedCount{0};
    DeadlockPolicy policy{DeadlockPolicy::WARN_ONLY};

    DeadlockRegistry();

    uint64_t getCurrentThreadId() const;
    ThreadState& getThreadState();

public:
    DeadlockRegistry(const DeadlockRegistry&) = delete;
    DeadlockRegistry& operator=(const DeadlockRegistry&) = delete;

    static DeadlockRegistry& instance();

    void setVerbose(bool enable);
    void setPolicy(DeadlockPolicy newPolicy);
    int getDeadlockCount() const;
    void reset();

    void registerLock(int lockId, const std::string& name);
    bool beforeLock(int lockId, const std::string& name);
    void afterLock(int lockId, const std::string& name);
    void beforeUnlock(int lockId, const std::string& name);

    Reporter& getReporter();
};
