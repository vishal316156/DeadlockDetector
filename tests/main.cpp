#include <iostream>
#include <thread>
#include <chrono>
#include <cassert>
#include <stdexcept>
#include "DeadlockDetector.hpp"

void testSafeExecution() {
    DeadlockRegistry::instance().reset();
    DeadlockRegistry::instance().setPolicy(DeadlockPolicy::WARN_ONLY);

    Mutex lockA("DB_Lock");
    Mutex lockB("Network_Lock");

    auto worker = [&]() {
        LockGuard g1(lockA);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        LockGuard g2(lockB);
    };

    std::thread t1(worker);
    std::thread t2(worker);
    t1.join();
    t2.join();

    assert(DeadlockRegistry::instance().getDeadlockCount() == 0);
    std::cout << "[PASS] testSafeExecution\n";
}

void testTwoThreadLockInversion() {
    DeadlockRegistry::instance().reset();
    DeadlockRegistry::instance().setPolicy(DeadlockPolicy::WARN_ONLY);

    Mutex lockA("Order_Lock");
    Mutex lockB("Payment_Lock");

    auto worker1 = [&]() {
        LockGuard g1(lockA);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        LockGuard g2(lockB);
    };

    auto worker2 = [&]() {
        LockGuard g1(lockB);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        LockGuard g2(lockA);
    };

    std::thread t1(worker1);
    t1.join();
    std::thread t2(worker2);
    t2.join();

    assert(DeadlockRegistry::instance().getDeadlockCount() == 1);
    std::cout << "[PASS] testTwoThreadLockInversion\n";
}

void testThreeThreadCircularWait() {
    DeadlockRegistry::instance().reset();
    DeadlockRegistry::instance().setPolicy(DeadlockPolicy::WARN_ONLY);

    Mutex lockA("Resource_A");
    Mutex lockB("Resource_B");
    Mutex lockC("Resource_C");

    auto worker1 = [&]() {
        LockGuard g1(lockA);
        LockGuard g2(lockB);
    };

    auto worker2 = [&]() {
        LockGuard g1(lockB);
        LockGuard g2(lockC);
    };

    auto worker3 = [&]() {
        LockGuard g1(lockC);
        LockGuard g2(lockA);
    };

    std::thread t1(worker1);
    t1.join();
    std::thread t2(worker2);
    t2.join();
    std::thread t3(worker3);
    t3.join();

    assert(DeadlockRegistry::instance().getDeadlockCount() == 1);
    std::cout << "[PASS] testThreeThreadCircularWait\n";
}

void testExceptionPolicy() {
    DeadlockRegistry::instance().reset();
    DeadlockRegistry::instance().setPolicy(DeadlockPolicy::THROW_EXCEPTION);

    Mutex lockA("Lock_X");
    Mutex lockB("Lock_Y");

    {
        LockGuard g1(lockA);
        LockGuard g2(lockB);
    }

    bool caught = false;
    try {
        LockGuard g1(lockB);
        LockGuard g2(lockA);
    } catch (const std::runtime_error&) {
        caught = true;
    }

    assert(caught);
    std::cout << "[PASS] testExceptionPolicy\n";
}

int main() {
    testSafeExecution();
    testTwoThreadLockInversion();
    testThreeThreadCircularWait();
    testExceptionPolicy();
    std::cout << "\nAll 4 test scenarios passed successfully.\n";
    return 0;
}
