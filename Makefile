CC = gcc
CXX = g++
.PHONY:all clean

# SOURCES := $(shell find src -name "*.cc")
SOURCES = $(shell find ./include/ -name "*.cc")
OBJECTS = $(SOURCES:.cc=.o)
CXXFLAGS = -std=c++11 -I ./include/

LDFLAGS = -lallegro -lallegro_main -lallegro_primitives
LDFLAGS += -lSDL2 -lGL
LDFLAGS += -lglfw
LDFLAGS += -lGLEW 

TARGET = build/miPSX

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
.cc.o:
	$(CXX) $(CXXFLAGS) $< -c -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)
run:
	@cd build && ./miPSX

