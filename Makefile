PREFIX ?= $(CURDIR)/bin
CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall
LDFLAGS ?= -lz
SRC = AdaptClean.cpp
TARGET = $(PREFIX)/AdaptClean

.PHONY: all clean

# Default target: compile binary directly in bin/
all: $(TARGET)

# Build rule
$(TARGET): $(SRC)
	@mkdir -p $(PREFIX)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)
	@echo "Built $(TARGET)"

# Clean rule
clean:
	rm -f $(TARGET)