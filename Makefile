CC = gcc
CXX = g++
.PHONY:all clean

SOURCES := $(shell find src -name "*.cc")
OBJECTS := $(SOURCES:.cc=.o)
CXXFLAGS := -std=c++11 -I include/
TARGET := build/miPSX

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@
.cc.o:
	$(CXX) $(CXXFLAGS) $< -c -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)
run:
	@cd build && ./miPSX