LIBNAME = zed
VERSION = 1
VER_SUB = 3

SRCS = $(wildcard z/*.cpp) $(wildcard z/core/*.cpp) $(wildcard z/file/*.cpp) $(wildcard z/math/*.cpp) $(wildcard z/system/*.cpp) $(wildcard z/util/*.cpp) $(wildcard z/util/regex/*.cpp) $(wildcard z/util/generic/*.cpp) $(wildcard z/util/dictionary/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

LIBFULL = $(LIBNAME).$(VERSION).$(VER_SUB)

ARCH = $(shell g++ -dumpmachine)

ifeq ($(findstring x86_64,$(ARCH)),x86_64)
CCTARGET = -m64
else
ifeq ($(findstring i686,$(ARCH)),i686)
CCTARGET = -m32
else
CCTARGET =
endif
endif

INCLUDE = -I"../libzed"
CCFLAGS = $(INCLUDE) -std=c++11 -W -Wall -Wextra -pedantic -fexceptions $(CCTARGET)
LFLAGS = -shared

STATIC_LIB = $(LIBNAME).a
DLL = $(LIBNAME).dll

# opt defaults to -O3
ifndef OPT
OLEVEL = 3
endif

#if opt flag is true
ifneq (,$(findstring $(OPT),S size Size SIZE))
OLEVEL = s
endif

# if debug flag is false
ifeq (,$(findstring $(DEBUG),1 true True TRUE))
CCFLAGS += -O$(OLEVEL) -g0
LFLAGS += -s
else
CCFLAGS += -g3 -O$(OLEVEL) -DDEBUG
endif

ifeq ($(OS),Windows_NT)
CFLAGS = $(CCFLAGS) -shared
LFLAGS += -fPIC
RMOBJS = $(subst /,\,$(OBJS))
RM = del
SHARED_LIB = $(LIBNAME).dll
else
CFLAGS = $(CCFLAGS) -fPIC
LFLAGS += -ldl
RMOBJS = $(OBJS)
RM = rm -f
SHARED_LIB = lib$(LIBNAME)-$(VERSION).$(VER_SUB).so
endif

DLFLAGS_WIN = -L. -l$(LIBNAME)
DLFLAGS_NIX = -l $(LIBNAME)

SONAME1 = lib$(LIBNAME).so.$(VERSION)
SONAME2 = lib$(LIBNAME).so

LIBDIR = /usr/lib

CC = g++
LN = g++

default: $(SHARED_LIB)

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

driver.exe: main.o
	$(LN) $(DLFLAGS_WIN) -o $@ $^

$(SHARED_LIB): $(OBJS)
	$(LN) -o $@ $^ $(LFLAGS)

main.o: main.cpp
	$(CC) $(CCFLAGS) -o $@ -c $^

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -o $@ -c $<

z/core/string.o: z/core/string.cpp z/core/string.h $(wildcard z/core/string/*.h)
	$(CC) $(CFLAGS) -o $@ -c $<

clean: clear
	$(RM) $(RMOBJS) main.o

clear:
	$(RM) driver *.so *.a *.dll *.exe

rebuild: clean default

.PHONY: rebuild clean clear default install uninstall
