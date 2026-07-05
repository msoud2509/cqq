CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic -Iinclude

BUILD_DIR := build
TARGET := $(BUILD_DIR)/cqq
SRCS := main.cc $(wildcard src/*.cc)
OBJS := $(SRCS:%.cc=$(BUILD_DIR)/%.o)

.PHONY: all run clean

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cc
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
