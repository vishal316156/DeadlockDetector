#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include "LockNode.hpp"

class LockGraph {
private:
    std::unordered_map<int, std::string> lockNames;
    std::unordered_map<int, std::unordered_set<int>> adj;

    bool dfs(int current, int target, std::unordered_set<int>& visited, std::vector<int>& path);

public:
    void registerLock(int id, const std::string& name);
    std::string getLockName(int id) const;
    void addEdge(int from, int to);
    CycleInfo checkDeadlock(int heldLockId, int requestedLockId, uint64_t threadId);
};
