CC = g++
CFLAGS = -I"../zLibraries" -std=c++11 -g -Wall -fexceptions
LFLAGS =

SRCS = main.cpp $(wildcard z/*.cpp) $(wildcard z/core/*.cpp) $(wildcard z/file/*.cpp) $(wildcard z/math/*.cpp) $(wildcard z/system/*.cpp) $(wildcard z/util/*.cpp)

OBJS = $(patsubst %.cpp,%.o,$(SRCS))

.PHONY: all
all: driver

driver: $(OBJS)
	g++ $(LFLAGS) -o driver $^

main.o: main.cpp
	g++ $(CFLAGS) -o $@ -c $^

%.o: %.cpp %.h
	g++ $(CFLAGS) -o $@ -c $<


.PHONY: clean
clean:
	rm -f $(OBJS) driver

.PHONY: clear
clear:
	rm -f $(OBJS)

.PHONY: rebuild
rebuild: clean all

# Below this is for comipling shared libraries


#all: lib exe

#lib:
#	g++ -shared -fPIC -o libtest.so dynamicLib.cpp

#exe:
#	g++ -o loadLib loadLib.cpp -ldl

#clean:
#	rm -f loadLib libtest.so.1.0 dynamicLib.o libtest.so
