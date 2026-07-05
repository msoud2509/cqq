CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic -Iinclude

BUILD_DIR := build
TARGET := $(BUILD_DIR)/cqq
SRCS := main.cc $(wildcard src/*.cc)
OBJS := $(SRCS:%.cc=$(BUILD_DIR)/%.o)

.PHONY: all run format clean

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

format:
	find . \( -name "*.cc" -o -name "*.cpp" -o -name "*.h" \) -exec clang-format -i {} +

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
