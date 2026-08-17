CXX ?= c++
CXXFLAGS ?= -O2
CPPFLAGS ?=

TEST_CPPFLAGS := -I CAST128_Example
TEST_CXXFLAGS := -std=c++11 -Wall -Wextra -Wpedantic -Werror
TEST_BINARY := build/cast128_test

.PHONY: all test clean

all: test

$(TEST_BINARY): tests/cast128_test.cpp CAST128_Example/cast128.cpp CAST128_Example/cast128.h
	mkdir -p build
	$(CXX) $(CPPFLAGS) $(TEST_CPPFLAGS) $(CXXFLAGS) $(TEST_CXXFLAGS) \
		tests/cast128_test.cpp CAST128_Example/cast128.cpp -o $@

test: $(TEST_BINARY)
	./$(TEST_BINARY)

clean:
	$(RM) -r build
