CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2 -Iinclude
LDFLAGS ?= -pthread

SRCS = src/LockGraph.cpp \
       src/Reporter.cpp \
       src/DeadlockRegistry.cpp \
       src/DeadlockDetector.cpp

TEST_SRCS = tests/main.cpp

TARGET = deadlock_test

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SRCS) $(TEST_SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) $(TEST_SRCS) -o $(TARGET) $(LDFLAGS)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe *.o
