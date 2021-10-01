
ifeq ($(OS),Windows_NT)
LIBEXT = dll
else
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

LFLAGS = -ldl
STATICLIB = ../libzed.a
STD = c++17

SRCS := $(wildcard source/*.cpp)
SRCS := $(filter-out source/dynamicLib.cpp,$(SRCS))
BINS = $(patsubst source/%.cpp,binary/%,$(SRCS))

all: binary object $(BINS)

binary:
	mkdir $@

object:
	mkdir $@

object/%.o: source/%.cpp
	g++ -I"../" -std=$(STD) -Wno-psabi -fPIC -o $@ -c $^

object/loadLib.o: source/loadLib.cpp binary/dynamicLib.$(LIBEXT)
	g++ -I"../" -std=$(STD) -Wno-psabi -fPIC -o $@ -c $<

binary/dynamicLib.$(LIBEXT): source/dynamicLib.cpp $(STATICLIB)
	g++ -I"../" -std=$(STD) -shared -fPIC -o $@ $^

binary/%: object/%.o $(STATICLIB)
	g++ -o $@ $^ $(LFLAGS)

clean:
	$(RM) binary object

.PHONY: all clean
