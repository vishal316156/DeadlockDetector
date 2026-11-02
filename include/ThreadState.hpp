#pragma once

#include <vector>
#include <cstdint>

struct ThreadState {
    uint64_t threadId = 0;
    std::vector<int> heldLockIds;
};
