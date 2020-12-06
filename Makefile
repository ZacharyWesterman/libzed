LIBNAME = zed
VERSION = 1
VER_SUB = 5

LIBDIR = /usr/lib
ICLDIR = /usr/include

D0 = $(sort $(dir $(wildcard z/*/)))
D1 = $(sort $(dir $(wildcard $(D0)*/)))
DIRS = $(sort $(dir $(wildcard $(D1)*/)))
SRCS = $(wildcard $(addsuffix *.cpp, $(DIRS)))
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

LIBFULL = $(LIBNAME).$(VERSION).$(VER_SUB)

ARCH = $(shell g++ -dumpmachine)

BITS =
ifeq ($(findstring x86_64,$(ARCH)),x86_64)
BITS = 64
else
ifeq ($(findstring i686,$(ARCH)),i686)
BITS = 32
endif
endif
ifeq ($(BITS),)
CCTARGET =
else
CCTARGET = -m$(BITS)
endif

STD = c++11

CCFLAGS = -std=$(STD) -W -Wall -Wextra -pedantic -fexceptions $(CCTARGET)
LFLAGS = -shared

STATIC_LIB = $(LIBNAME).a
DLL = $(LIBNAME).dll

OLEVEL = $(OPT)

# opt defaults to -O3
ifndef OPT
OLEVEL = 3
endif

ifneq (,$(findstring $(OPT),S size Size SIZE))
OLEVEL = s
endif
ifneq (,$(findstring $(OPT),F f Fast FAST))
OLEVEL = fast
endif

# if debug flag is not set
ifndef DEBUG
CCFLAGS += -O$(OLEVEL) -g0
LFLAGS += -s
endif
ifdef DEBUG
CCFLAGS += -g$(DEBUG) -O$(OLEVEL) -DDEBUG
endif

# if rm exists use that, otherwise try Windows' "del"
ifeq (, $(shell rm --version))
RM = del
else
RM = rm -f
endif

ifeq ($(OS),Windows_NT)
CFLAGS = $(CCFLAGS) -shared
LFLAGS += -fPIC
RMOBJS = $(subst /,\,$(OBJS))
SHARED_LIB = $(LIBNAME).dll
else
CFLAGS = $(CCFLAGS) -fPIC
LFLAGS += -ldl
RMOBJS = $(OBJS)
SHARED_LIB = lib$(LIBNAME)-$(VERSION).$(VER_SUB).so
endif

DLFLAGS_WIN = -L. -l$(LIBNAME)
DLFLAGS_NIX = -l $(LIBNAME)

SONAME1 = lib$(LIBNAME).so.$(VERSION)
SONAME2 = lib$(LIBNAME).so

CC = g++
LN = g++

default: $(SHARED_LIB)

install: $(SHARED_LIB)
	cp $(SHARED_LIB) $(LIBDIR)/$(SHARED_LIB)
	rm -f $(LIBDIR)/$(SONAME1) $(LIBDIR)/$(SONAME2)
	ln -s $(LIBDIR)/$(SHARED_LIB) $(LIBDIR)/$(SONAME1)
	ln -s $(LIBDIR)/$(SHARED_LIB) $(LIBDIR)/$(SONAME2)
	ldconfig
	find z -name '*.hpp' -exec cp --parents '{}' $(ICLDIR) ';'

uninstall:
	rm -f $(LIBDIR)/lib$(LIBNAME)*
	ldconfig
	rm -rf $(ICLDIR)/z

examples:
	$(MAKE) -C examples/

$(SHARED_LIB): $(OBJS)
	$(LN) -o $@ $^ $(LFLAGS)

%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -o $@ -c $<

z/core/string.o: z/core/string.cpp z/core/string.hpp $(wildcard z/core/string/*.hpp)
	$(CC) $(CFLAGS) -o $@ -c $<

z/file/library.o: z/file/library.cpp z/file/library.hpp
	$(CC) $(CFLAGS) -DZ_DYNLIB -o $@ -c $<

clean: cleanbin cleanobjs

cleanobjs:
	$(RM) $(RMOBJS)

cleanbin:
	$(RM) driver
	$(RM) lib$(LIBNAME)-*.so
	$(RM) $(LIBNAME).dll

rebuild: clean default

.PHONY: rebuild clean cleanobjs cleanbin default install uninstall examples
