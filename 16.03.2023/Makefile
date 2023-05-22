CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

OBJS = main.o manager.o

.PHONY: all clean

all: a.out

a.out: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o a.out

main.o: main.cpp manager.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

manager.o: manager.cpp manager.hpp
	$(CXX) $(CXXFLAGS) -c manager.cpp

clean:
	rm -f $(OBJS) a.out
