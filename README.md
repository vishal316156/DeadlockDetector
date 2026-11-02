# DeadlockDetector

A lightweight C++ runtime library for detecting potential deadlocks and lock-order inversions using dynamic directed graph cycle detection.

---

## Overview

Deadlocks typically freeze multithreaded systems silently at runtime. `DeadlockDetector` intercepts synchronization operations via RAII wrappers (`Mutex`, `LockGuard`) and tracks resource dependency graphs:

1. **Lock-Order Validation:** Detects circular wait conditions at runtime before threads block permanently.
2. **Low Overhead:** Uses `thread_local` state for held locks to avoid cross-thread synchronization during normal lock acquisitions.
3. **Configurable Policies:** Supports non-fatal diagnostic warnings (`WARN_ONLY`) or fail-fast exceptions (`THROW_EXCEPTION`).

---

## Project Structure

```text
DeadLockDetector/
├── include/
│   ├── DeadlockDetector.hpp   # Public Mutex and LockGuard interfaces
│   ├── DeadlockRegistry.hpp   # Internal coordinator and policy manager
│   ├── LockGraph.hpp          # Directed graph and DFS cycle-finding engine
│   ├── LockNode.hpp           # Data definitions (CycleInfo, LockNode)
│   ├── ThreadState.hpp        # Thread-local lock tracking structures
│   └── Reporter.hpp           # Diagnostic logging
├── src/
│   ├── DeadlockDetector.cpp
│   ├── DeadlockRegistry.cpp
│   ├── LockGraph.cpp
│   └── Reporter.cpp
├── tests/
│   └── main.cpp
├── CMakeLists.txt
├── Makefile
├── .gitignore
└── README.md
```

---

## Building and Testing

### Using CMake
```bash
cmake -B build
cmake --build build

# Run test binary
./build/deadlock_test          # Linux / macOS
.\build\deadlock_test.exe      # Windows
```

Run test suite via CTest:
```bash
ctest --test-dir build --output-on-failure
```

### Using Make
```bash
make          # or mingw32-make on Windows
make test     # build and execute tests
make clean    # remove binaries
```
