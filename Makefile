# Makefile for ChessCrafters

# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -Wall -std=c++11 -Iinclude

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Target executable name
TARGET := $(BIN_DIR)/chess_game

# Source and object files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default make command
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for cleaning up generated files
clean:
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)/*

.PHONY: all clean run

# Default make command
all: build run

build: $(TARGET)

# Rule for running the program
run: build
	./$(TARGET)
