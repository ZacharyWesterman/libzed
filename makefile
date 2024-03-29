LIBNAME = zed
VERSION = 1
VER_SUB = 6

LIBDIR = /usr/lib
ICLDIR = /usr/include

D0 = $(sort $(dir $(wildcard z/*/)))
D1 = $(sort $(dir $(wildcard $(D0)*/)))
DIRS = $(sort $(dir $(wildcard $(D1)*/)) $(D0) $(D1) )
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


#Generate for specified std and arch,
#Show all warnings & handle exceptions,
#Let compiler know we're making a library,
#And separate the data & function sections so that unused symbols can be stripped.
CCFLAGS = -std=$(STD) $(CCTARGET) \
	-W -Wall -Wextra -Wno-psabi -Werror \
	-pedantic -fexceptions \
	-fPIC \
	-fdata-sections -ffunction-sections

LFLAGS = -shared $(CCTARGET)

STD = c++17

STATIC_LIB = lib$(LIBNAME).a

OLEVEL = $(OPT)

# opt defaults to -O3
ifndef OPT
OLEVEL = 3
endif

ifneq (,$(findstring $(OPT),F f Fast FAST))
OLEVEL = fast
endif
ifneq (,$(findstring $(OPT),S s size Size SIZE))
OLEVEL = s
endif

# if debug flag is not set
ifndef DEBUG
CCFLAGS += -O$(OLEVEL)
LFLAGS += -s
endif
ifdef DEBUG
CCFLAGS += -g$(DEBUG) -O$(OLEVEL) -DDEBUG
endif

# if rm exists use that, otherwise try Windows' "del"
ifeq (, $(shell rm --version))
RM = del
RMDIR = rd /s /q
else
RM = rm -f
RMDIR = rm -rf
endif

ifeq ($(OS),Windows_NT)
RMOBJS = $(subst /,\,$(OBJS))
SHARED_LIB = $(LIBNAME).dll
else
# link to std::filesystem if c++17 and linux
ifneq (,$(findstring $(STD),c++17 gnu++17 c++20 gnu++20))
LFLAGS += -lstdc++fs
endif
LFLAGS += -ldl
RMOBJS = $(OBJS)
SHARED_LIB = lib$(LIBNAME)-$(VERSION).$(VER_SUB).so
endif

SONAME1 = lib$(LIBNAME).so.$(VERSION)
SONAME2 = lib$(LIBNAME).so

default: dynamic

all: dynamic static examples lint tests

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

examples: static
	$(MAKE) -C examples/

tests: static
	$(MAKE) -C tests/
	./tests/bin/run_tests

$(SHARED_LIB): $(OBJS)
	$(LN) -o $@ $^ $(LFLAGS)

$(STATIC_LIB): $(OBJS)
	ar rcs $@ $^
	ranlib $@

%.o: %.cpp %.hpp
	$(CC) $(CCFLAGS) -o $@ -c $<

z/core/string.o: z/core/string.cpp z/core/string.hpp $(wildcard z/core/string/*.hpp)
	$(CC) $(CCFLAGS) -o $@ -c $<

z/file/library.o: z/file/library.cpp z/file/library.hpp
	$(CC) $(CCFLAGS) -DZ_DYNLIB -o $@ -c $<

clean: cleanbin cleanobjs cleandox cleancov
	$(MAKE) clean -C examples/
	$(MAKE) clean -C tests/

cleanobjs:
	$(RM) $(RMOBJS)

cleanbin:
	$(RM) driver *.so *.dll *.a

cleancov:
	$(RM) $(patsubst %.cpp,%.gcno,$(SRCS))

cleandox:
	$(RMDIR) html
	$(RM) *.log

rebuild: clean default

lint:
	clang-tidy -header-filter=.* $(SRCS) -- $(CCFLAGS) -Wno-unused-private-field >lint.log

docs:
	wget https://raw.githubusercontent.com/jothepro/doxygen-awesome-css/main/doxygen-awesome.css
	doxygen
	rm doxygen-awesome.css

.PHONY: rebuild clean cleanobjs cleanbin cleancov cleandox default install uninstall examples static dynamic shared all lint tests docs
