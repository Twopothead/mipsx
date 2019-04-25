
CXX = g++
CXXFLAGS = -std=c++11 #-I ./include 
LDFLAGS = -lSDL2 -lGL -lGLEW

.PHONY:all clean run

all:
	$(CXX) $(CXXFLAGS) test_glew.cc $(LDFLAGS) -o test_glew

clean:
	rm -f test_glew

run:
	./test_glew