#include "DeadlockRegistry.hpp"
#include <thread>
#include <algorithm>
#include <stdexcept>

thread_local ThreadState localThreadState;

DeadlockRegistry::DeadlockRegistry() : reporter(false) {}

DeadlockRegistry& DeadlockRegistry::instance() {
    static DeadlockRegistry inst;
    return inst;
}

uint64_t DeadlockRegistry::getCurrentThreadId() const {
    return std::hash<std::thread::id>{}(std::this_thread::get_id());
}

ThreadState& DeadlockRegistry::getThreadState() {
    if (localThreadState.threadId == 0) {
        localThreadState.threadId = getCurrentThreadId();
    }
    return localThreadState;
}

void DeadlockRegistry::setVerbose(bool enable) {
    reporter.setVerbose(enable);
}

void DeadlockRegistry::setPolicy(DeadlockPolicy newPolicy) {
    policy = newPolicy;
}

int DeadlockRegistry::getDeadlockCount() const {
    return deadlockDetectedCount.load();
}

void DeadlockRegistry::reset() {
    std::lock_guard<std::mutex> lock(internalMutex);
    graph = LockGraph();
    deadlockDetectedCount = 0;
    localThreadState.heldLockIds.clear();
}

void DeadlockRegistry::registerLock(int lockId, const std::string& name) {
    std::lock_guard<std::mutex> lock(internalMutex);
    graph.registerLock(lockId, name);
}

bool DeadlockRegistry::beforeLock(int lockId, const std::string& name) {
    uint64_t tid = getCurrentThreadId();
    ThreadState& state = getThreadState();

    bool cycleFound = false;

    {
        std::lock_guard<std::mutex> lock(internalMutex);
        graph.registerLock(lockId, name);

        for (int heldId : state.heldLockIds) {
            CycleInfo info = graph.checkDeadlock(heldId, lockId, tid);
            if (info.hasCycle) {
                deadlockDetectedCount++;
                reporter.reportDeadlock(info);
                cycleFound = true;
                if (policy == DeadlockPolicy::THROW_EXCEPTION) {
                    throw std::runtime_error("Deadlock cycle detected on lock: " + name);
                }
            }
            graph.addEdge(heldId, lockId);
        }
    }

    return cycleFound;
}

void DeadlockRegistry::afterLock(int lockId, const std::string& name) {
    ThreadState& state = getThreadState();
    state.heldLockIds.push_back(lockId);
    reporter.reportAcquire(state.threadId, name);
}

void DeadlockRegistry::beforeUnlock(int lockId, const std::string& name) {
    ThreadState& state = getThreadState();
    auto it = std::find(state.heldLockIds.rbegin(), state.heldLockIds.rend(), lockId);
    if (it != state.heldLockIds.rend()) {
        state.heldLockIds.erase(std::next(it).base());
    }
    reporter.reportRelease(state.threadId, name);
}

Reporter& DeadlockRegistry::getReporter() {
    return reporter;
}
