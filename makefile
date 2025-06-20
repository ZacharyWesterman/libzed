LIBNAME = zed
VER_MAJOR = 1
VER_MINOR = 9
VER_CUTOFF_COMMIT = 8ef9abcd3e99a59f1dc0ea489b0c38b416ca82fb

#Auto-calculate patch version based on current commit.
#If patch version can't be calculated, just use the OS name.
LATEST_COMMIT = $(shell git rev-parse HEAD)
ifeq ($(LATEST_COMMIT),)
ifeq ($(OS),Windows_NT)
VER_PATCH = Windows
else
VER_PATCH = Linux
endif
else
VER_PATCH = $(shell git rev-list --count $(VER_CUTOFF_COMMIT)..HEAD^)
endif

LIBDIR = /usr/lib
ICLDIR = /usr/include

D0 = $(sort $(dir $(wildcard z/*/)))
D1 = $(sort $(dir $(wildcard $(D0)*/)))
DIRS := $(sort $(dir $(wildcard $(D1)*/)) $(D0) $(D1) )
SRCS := $(wildcard $(addsuffix *.cpp, $(DIRS)))
HEADERS := $(wildcard $(addsuffix *.hpp, $(DIRS))) $(wildcard z/*.hpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
DEPENDS := $(patsubst %.cpp,%.d,$(SRCS))

LIBFULL = $(LIBNAME).$(VER_MAJOR).$(VER_MINOR)

#default compiler
CC = g++

#Little macro for converting text to lowercase
#It's gross but it works on all systems without installing extra libs!
lc = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))

#Linux is default target
TGT := $(call lc,$(TARGET))
ifneq ($(filter $(TGT),lin nix linux),)
TGT :=
endif

#If we're cross-compiling for Windows.
ifneq ($(filter $(TGT),win64 win windows),)
CC = x86_64-w64-mingw32-g++
else
ifeq ($(TGT),win32)
CC = i686-w64-mingw32-g++
else

#If not windows and not linux, error.
ifneq ($(TGT),)
$(error ERROR: Invalid `TARGET=...` flag. Should be one of `win64`, `win32`, or `linux`)
endif

endif
endif

LN = $(CC)

#If we're cross-compiling for Windows, set the OS variable to Windows_NT.
ifneq (,$(findstring mingw,$(CC)))
OS = Windows_NT
endif

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
	-fdata-sections -ffunction-sections \
	$(EXTRA_CFLAGS)

LFLAGS = -shared $(CCTARGET) $(EXTRA_LFLAGS)

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
#In Windows, some versions of GCC will falsely complain about "writing 1 byte into a region of size 0". Disable this warning.
Z_STRING_FLAGS = -Wno-stringop-overflow
else
# link to std::filesystem if c++17 and linux
ifneq (,$(findstring $(STD),c++17 gnu++17 c++20 gnu++20))
LFLAGS += -lstdc++fs
endif
LFLAGS += -ldl
RMOBJS = $(OBJS)
SHARED_LIB = lib$(LIBNAME)-$(VER_MAJOR).$(VER_MINOR).so
endif

SONAME1 = lib$(LIBNAME).so.$(VER_MAJOR)
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

test: tests
tests: static
	$(MAKE) -C tests/ STD=$(STD)
	./tests/bin/run_tests $(TEST_FLAGS)

benchmark: benchmarks
benchmarks: static
	$(MAKE) -C benchmarks/ STD=$(STD)
	./benchmarks/bin/run_benchmarks $(TEST_FLAGS)

$(SHARED_LIB): $(OBJS)
	$(LN) -o $@ $^ $(LFLAGS)

$(STATIC_LIB): $(OBJS)
	ar rcs $@ $^
	ranlib $@

-include $(DEPENDS)

%.o: %.cpp %.hpp makefile
	$(CC) $(CCFLAGS) -MMD -MP -o $@ -c $<

z/core/string.o: z/core/string.cpp z/core/string.hpp $(wildcard z/core/string/*.hpp)
	$(CC) $(CCFLAGS) $(Z_STRING_FLAGS) -MMD -MP -o $@ -c $<

z/file/library.o: z/file/library.cpp z/file/library.hpp
	$(CC) $(CCFLAGS) -MMD -MP -o $@ -c $<

clean: cleanbin cleanobjs cleandox cleancov
	$(MAKE) clean -C examples/
	$(MAKE) clean -C tests/

cleanobjs:
	$(RM) $(RMOBJS) $(DEPENDS)

cleanbin:
	$(RM) driver *.so *.dll *.a

cleancov:
	$(RM) $(patsubst %.cpp,%.gcno,$(SRCS))

cleandox:
	$(RMDIR) html
	$(RM) *.log
	$(RM) Doxypages/examples.dox

rebuild: clean default

lint: lint.log
	@cat $^

lint.log: $(HEADERS) $(wildcard examples/src/*.cpp)
	@find z/ -type f \( -name '*.cpp' -or -name '*.hpp' \) -not -name '*Constructors.hpp' -not -name 'utf*.hpp' -not -name 'ascii.hpp' -not -name 'shared.hpp' | xargs -P8 -I{} clang-tidy {} -header-filter=.* -- -std=c++17 -m64 -W -Wall -Wextra -Wno-psabi -Werror -pedantic -fexceptions -fPIC -fdata-sections -ffunction-sections -O3 -Wno-unused-private-field > lint.log 2>/dev/null || { cat $@; [ "$$(cat $@)" = '' ] && echo 'ERROR: Is clang-tidy installed?' && rm $@ -f; exit 1; }

format:
	find . -type f \( -name '*.cpp' -or -name '*.hpp' \) -not -name 'catch_amalgamated.*' | xargs -P8 -I{} sh -c 'echo Formatting {}; clang-format -i {}'

try-format:
	@find . -type f \( -name '*.cpp' -or -name '*.hpp' \) -not -name 'catch_amalgamated.*' | xargs -P8 -I{} sh -c 'clang-format --dry-run -Werror -i {}'

dox: docs
docs: html
	@cat doxygen.log

html: $(HEADERS) Doxyfile $(wildcard Doxypages/*.dox) Doxypages/examples.dox $(wildcard examples/src/*.cpp) README.md
	$(RMDIR) html
	PROJECT_NUMBER=$(VER_MAJOR).$(VER_MINOR).$(VER_PATCH) doxygen

Doxypages/examples.dox: examples/src/*.cpp
	@{ echo '/**'; for file in $^; do echo "@example $$(basename $$file)"; done; echo '*/'; } > $@

count-loc:
	@find z -type f \( -name "*.cpp" -o -name "*.hpp" \) -exec wc -l {} +

get-version:
	@echo $(VER_MAJOR).$(VER_MINOR).$(VER_PATCH)

get-revision:
	git rev-parse HEAD

.PHONY: rebuild clean cleanobjs cleanbin cleancov cleandox default install uninstall examples static dynamic shared all lint test tests benchmark benchmarks docs dox format try-format count-loc get-version get-revision
