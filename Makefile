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

CC = g++
LN = $(CC)

ARCH = $(shell $(CC) -dumpmachine)

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

#Generate for specified std and arch,
#Show all warnings,
#Let compiler know we're making a library,
#And separate the data & function sections so that unused symbols can be stripped.
CCFLAGS = -std=$(STD) $(CCTARGET) \
	-W -Wall -Wextra -pedantic -fexceptions \
	-fPIC \
	-fdata-sections -ffunction-sections

LFLAGS = -shared $(CCTARGET)

STATIC_LIB = $(LIBNAME).a

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
RMOBJS = $(subst /,\,$(OBJS))
SHARED_LIB = $(LIBNAME).dll
else
LFLAGS += -ldl
RMOBJS = $(OBJS)
SHARED_LIB = lib$(LIBNAME)-$(VERSION).$(VER_SUB).so
endif

SONAME1 = lib$(LIBNAME).so.$(VERSION)
SONAME2 = lib$(LIBNAME).so

default: $(SHARED_LIB)

static: $(STATIC_LIB)
dynamic: $(SHARED_LIB)
shared: $(SHARED_LIB)

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

$(STATIC_LIB): $(OBJS)
	ar rcs lib$(LIBNAME).a $(OBJS)
	ranlib lib$(LIBNAME).a

%.o: %.cpp %.hpp
	$(CC) $(CCFLAGS) -o $@ -c $<

z/core/string.o: z/core/string.cpp z/core/string.hpp $(wildcard z/core/string/*.hpp)
	$(CC) $(CCFLAGS) -o $@ -c $<

z/file/library.o: z/file/library.cpp z/file/library.hpp
	$(CC) $(CCFLAGS) -DZ_DYNLIB -o $@ -c $<

clean: cleanbin cleanobjs

cleanobjs:
	$(RM) $(RMOBJS)

cleanbin:
	$(RM) driver *.so *.dll

rebuild: clean default

.PHONY: rebuild clean cleanobjs cleanbin default install uninstall examples static dynamic shared
