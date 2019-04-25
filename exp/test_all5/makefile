
CXX = g++
CXXFLAGS = -std=c++11 #-I ./include 
LDFLAGS = -lallegro -lallegro_main -lallegro_primitives

.PHONY:all clean run

all:
	$(CXX) $(CXXFLAGS) all5.cc $(LDFLAGS) -o test5

clean:
	rm -f test5

run:
	./test5


	# g++ all5.cc -lallegro -lallegro_main -lallegro_primitives 