
ifeq ($(OS),Windows_NT)
LIBEXT = dll
else
LIBEXT = so
LFLAGS = -ldl
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

STATICLIB = ../libzed.a
STD = c++17

SRCS := $(wildcard src/*.cpp)
SRCS := $(filter-out src/dynamicLib.cpp,$(SRCS))
BINS = $(patsubst src/%.cpp,bin/%,$(SRCS))

all: bin obj $(BINS)

bin:
	mkdir $@

obj:
	mkdir $@

obj/%.o: src/%.cpp
	g++ -I"../" -std=$(STD) -Wno-psabi -fPIC -o $@ -c $^

obj/loadLib.o: src/loadLib.cpp bin/dynamicLib.$(LIBEXT)
	g++ -I"../" -std=$(STD) -Wno-psabi -fPIC -o $@ -c $<

bin/dynamicLib.$(LIBEXT): src/dynamicLib.cpp $(STATICLIB)
	g++ -I"../" -std=$(STD) -shared -fPIC -o $@ $^

bin/%: obj/%.o $(STATICLIB)
	g++ -o $@ $^ $(LFLAGS)

clean:
	$(RM) bin obj

.PHONY: all clean
