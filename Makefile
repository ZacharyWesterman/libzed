LIBNAME = zed
VERSION = 1
VER_SUB = 1

SRCS = $(wildcard z/*.cpp) $(wildcard z/core/*.cpp) $(wildcard z/file/*.cpp) $(wildcard z/math/*.cpp) $(wildcard z/system/*.cpp) $(wildcard z/util/*.cpp) $(wildcard z/util/regex/*.cpp) $(wildcard z/util/generic/*.cpp) $(wildcard z/util/dictionary/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

LIBFULL = $(LIBNAME).$(VERSION).$(VER_SUB)

LBITS = $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
CCTARGET = -m64
else
CCTARGET = -m32
endif

INCLUDE = -I"../libzed"
CCFLAGS = $(INCLUDE) -std=c++11 -g -W -Wall -Wextra -pedantic -fexceptions -O4 $(CCTARGET)

STATIC_LIB = $(LIBNAME).a
DLL = $(LIBNAME).dll

LFLAGS = -s -shared

ifeq ($(OS),Windows_NT)
CFLAGS = $(CCFLAGS) -shared
else
CFLAGS = $(CCFLAGS) -fPIC
LFLAGS := -ldl
endif

DLFLAGS_WIN = -L. -l$(LIBNAME) -Wl,-rpath=. $(CCTARGET)
DLFLAGS_NIX = -l $(LIBNAME) $(CCTARGET)

SHARED_LIB = lib$(LIBNAME)-$(VERSION).$(VER_SUB).so
SONAME1 = lib$(LIBNAME).so.$(VERSION)
SONAME2 = lib$(LIBNAME).so

LIBDIR = /usr/lib

CC = g++
LN = g++

default: shared

all: shared static driver

install: $(SHARED_LIB)
	cp $(SHARED_LIB) $(LIBDIR)/$(SHARED_LIB)
	rm -f $(LIBDIR)/$(SONAME1) $(LIBDIR)/$(SONAME2)
	ln -s $(LIBDIR)/$(SHARED_LIB) $(LIBDIR)/$(SONAME1)
	ln -s $(LIBDIR)/$(SHARED_LIB) $(LIBDIR)/$(SONAME2)
	ldconfig

uninstall:
	rm -f $(LIBDIR)/lib$(LIBNAME)*
	ldconfig

driver: main.o
	$(LN) $(DLFLAGS_NIX) -o $@ $^

driver.exe: main.o $(DLL)
	$(LN) $(DLFLAGS_WIN) -o $@ 

shared: $(SHARED_LIB)
dynamic: $(SHARED_LIB)

$(SHARED_LIB): $(OBJS)
	$(LN) -o $@ $^ $(LFLAGS)

$(DLL): $(OBJS)
	$(LN) -o $@ $^ -Wl,--out-implib,$(STATIC_LIB)

main.o: main.cpp
	$(CC) $(CCFLAGS) -o $@ -c $^

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -o $@ -c $<

z/core/string.o: z/core/string.cpp z/core/string.h $(wildcard z/core/string/*.h)
	$(CC) $(CFLAGS) -o $@ -c $<

clean: clear
	rm -f $(OBJS) main.o

clear:
	rm -f driver *.so *.a *.dll *.exe

rebuild: clean all

.PHONY: rebuild clean clear all shared install uninstall static dynamic dllreq
