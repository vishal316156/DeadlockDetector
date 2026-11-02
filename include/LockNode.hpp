#pragma once

#include <string>
#include <vector>
#include <cstdint>

struct LockNode {
    int id;
    std::string name;
};

struct CycleInfo {
    bool hasCycle;
    std::vector<std::string> path;
    uint64_t threadId;
    std::string requestedLock;
};
