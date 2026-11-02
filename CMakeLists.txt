cmake_minimum_required(VERSION 3.15)
project(DeadlockDetector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_library(deadlock_detector STATIC
    src/LockGraph.cpp
    src/Reporter.cpp
    src/DeadlockRegistry.cpp
    src/DeadlockDetector.cpp
)

target_include_directories(deadlock_detector PUBLIC include)
target_link_libraries(deadlock_detector PUBLIC Threads::Threads)

add_executable(deadlock_test tests/main.cpp)
target_link_libraries(deadlock_test PRIVATE deadlock_detector)

enable_testing()
add_test(NAME DeadlockTest COMMAND deadlock_test)
