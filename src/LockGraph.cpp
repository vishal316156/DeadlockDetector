#include "LockGraph.hpp"

void LockGraph::registerLock(int id, const std::string& name) {
    if (lockNames.find(id) == lockNames.end()) {
        lockNames[id] = name;
    }
}

std::string LockGraph::getLockName(int id) const {
    auto it = lockNames.find(id);
    if (it != lockNames.end()) {
        return it->second;
    }
    return "UnknownLock_" + std::to_string(id);
}

void LockGraph::addEdge(int from, int to) {
    if (from != to) {
        adj[from].insert(to);
    }
}

bool LockGraph::dfs(int current, int target, std::unordered_set<int>& visited, std::vector<int>& path) {
    visited.insert(current);
    path.push_back(current);

    if (current == target) {
        return true;
    }

    auto it = adj.find(current);
    if (it != adj.end()) {
        for (int neighbor : it->second) {
            if (visited.find(neighbor) == visited.end()) {
                if (dfs(neighbor, target, visited, path)) {
                    return true;
                }
            }
        }
    }

    path.pop_back();
    return false;
}

CycleInfo LockGraph::checkDeadlock(int heldLockId, int requestedLockId, uint64_t threadId) {
    std::unordered_set<int> visited;
    std::vector<int> path;

    bool found = dfs(requestedLockId, heldLockId, visited, path);

    CycleInfo info;
    info.hasCycle = found;
    info.threadId = threadId;
    info.requestedLock = getLockName(requestedLockId);

    if (found) {
        for (int id : path) {
            info.path.push_back(getLockName(id));
        }
        info.path.push_back(getLockName(requestedLockId));
    }

    return info;
}
