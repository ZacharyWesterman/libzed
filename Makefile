SRCS = $(wildcard z/*.cpp) $(wildcard z/core/*.cpp) $(wildcard z/file/*.cpp) $(wildcard z/math/*.cpp) $(wildcard z/system/*.cpp) $(wildcard z/util/*.cpp) $(wildcard z/util/regex/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_RELEASE = 0

INCLUDE = -I"../zLibraries"
CCFLAGS = $(INCLUDE) -std=c++11 -g -Wall -fexceptions -O4
CFLAGS = $(CCFLAGS) -fPIC
LFLAGS = -ldl -s

DRIVER_FLAGS = -L. -lzed -Wl,-rpath=.

STATIC_LIB = zed.a
SHARED_LIB = libzed.so

CC = g++
LN = g++

default: shared

all: shared static driver

driver: main.o
	$(LN) $(DRIVER_FLAGS) -o $@ $^

shared: $(SHARED_LIB)
static: $(STATIC_LIB)

$(STATIC_LIB): $(OBJS)
	ar -cvq $@ $^

$(SHARED_LIB): $(OBJS)
	$(LN) $(LFLAGS) -shared -o $@ $^

main.o: main.cpp
	$(CC) $(CCFLAGS) -o $@ -c $^

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS) $(STATIC_LIB) driver main.o *.so

clear:
	rm -f $(OBJS) main.o

rebuild: clean all

.PHONY: rebuild clean clear all shared install uninstall driver
