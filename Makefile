CFLAGS= -I"../zLibraries" -std=c++11 -g -Wall -fexceptions

all:
	g++ $(CFLAGS) main.cpp -o zLibraries_driver

clean:
	rm -f zLibraries_driver



# Below this is for comipling shared libraries


#all: lib exe

#lib:
#	g++ -shared -fPIC -o libtest.so dynamicLib.cpp

#exe:
#	g++ -o loadLib loadLib.cpp -ldl

#clean:
#	rm -f loadLib libtest.so.1.0 dynamicLib.o libtest.so
