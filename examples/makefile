
ifeq ($(OS),Windows_NT)
LFLAGS = -L. -lzed
DLL1 = zed.dll
DLL2 = binary/zed.dll
LIBEXT = dll
else
LFLAGS = -lzed
LIBEXT = so
endif

ifeq (, $(shell rm --version))
RM = rd /s /q
MV = move
CP = copy "..\zed.dll"
else
RM = rm -rf
MV = mv
CP = cp "../zed.dll"
endif

STD = c++17

SRCS := $(wildcard source/*.cpp)
SRCS := $(filter-out source/dynamicLib.cpp,$(SRCS))
BINS = $(patsubst source/%.cpp,binary/%,$(SRCS))

all: binary object $(DLL1) binary/dynamicLib.$(LIBEXT) $(BINS) $(DLL2)

binary:
	mkdir $@

object:
	mkdir $@

object/%.o: source/%.cpp
	g++ -I"../" -std=$(STD) -Wno-psabi -fPIC -o $@ -c $^

binary/dynamicLib.$(LIBEXT): source/dynamicLib.cpp
	g++ -I"../" -std=$(STD) -shared -fPIC -o $@ $^

binary/%: object/%.o
	g++ $(LFLAGS) -o $@ $^

$(DLL2): $(DLL1)
	$(MV) "$^" "$@"

$(DLL1): ../$(DLL1)
	$(CP) "$@"

clean:
	$(RM) binary object

.PHONY: all clean
