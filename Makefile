PREFIX ?= $(HOME)/bin
CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall
LDFLAGS ?= -lz
SRC = AdaptClean.cpp
TARGET = AdaptClean

.PHONY: all install clean

all: $(TARGET) install

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

install: $(TARGET)
	mkdir -p "$(PREFIX)"
	cp $(TARGET) "$(PREFIX)"
	@echo "Installed $(TARGET) to $(PREFIX)"

clean:
	rm -f $(TARGET)
