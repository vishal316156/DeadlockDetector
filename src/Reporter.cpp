#include "Reporter.hpp"
#include <iostream>

Reporter::Reporter(bool verbose) : verbose(verbose) {}

void Reporter::setVerbose(bool enable) {
    verbose = enable;
}

void Reporter::reportDeadlock(const CycleInfo& info) const {
    std::cerr << "\n[DeadlockDetector] WARNING: Lock-order inversion detected\n"
              << "  Thread ID : " << info.threadId << "\n"
              << "  Acquiring : " << info.requestedLock << "\n"
              << "  Cycle     : ";
    for (size_t i = 0; i < info.path.size(); ++i) {
        std::cerr << info.path[i];
        if (i + 1 < info.path.size()) {
            std::cerr << " -> ";
        }
    }
    std::cerr << "\n\n";
}

void Reporter::reportAcquire(uint64_t threadId, const std::string& lockName) const {
    if (verbose) {
        std::cout << "[thread " << threadId << "] acquire: " << lockName << "\n";
    }
}

void Reporter::reportRelease(uint64_t threadId, const std::string& lockName) const {
    if (verbose) {
        std::cout << "[thread " << threadId << "] release: " << lockName << "\n";
    }
}

void Reporter::log(const std::string& message) const {
    std::cout << message << "\n";
}
