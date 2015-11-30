CXXFLAGS =-std=c++0x -ggdb -Wall -pedantic -ansi #-Werror
all: multi
multi:source.o
	g++ $(CXXFLAGS) source.o -o multi
source.o:source.cpp
	g++ $(CXXFLAGS) -c source.cpp
clean:
	rm -f *.o multi

