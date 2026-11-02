#pragma once

#include <string>
#include <cstdint>
#include "LockNode.hpp"

class Reporter {
private:
    bool verbose;

public:
    explicit Reporter(bool verbose = false);

    void setVerbose(bool enable);
    void reportDeadlock(const CycleInfo& info) const;
    void reportAcquire(uint64_t threadId, const std::string& lockName) const;
    void reportRelease(uint64_t threadId, const std::string& lockName) const;
    void log(const std::string& message) const;
};
