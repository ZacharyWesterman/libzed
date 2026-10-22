STATICLIB = ../libzed.a
STD = c++17
CCFLAGS := -std=$(STD) -Wno-psabi -fPIC

ifeq ($(OS),Windows_NT)
LIBEXT = dll
LFLAGS = -Wl,--gc-sections
else
LIBEXT = so
LFLAGS = -ldl -Wl,--gc-sections
endif

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

ifeq (, $(shell rm --version))
RM = rd /s /q
MV = move
CP = copy "..\zed.dll"
else
RM = rm -rf
MV = mv
CP = cp "../zed.dll"
endif

SRCS := $(wildcard src/*.cpp)
SRCS := $(filter-out src/dynamicLib.cpp,$(SRCS))
BINS = $(patsubst src/%.cpp,bin/%,$(SRCS))

all: bin obj $(BINS)

bin:
	mkdir $@

obj:
	mkdir $@

obj/%.o: src/%.cpp
	g++ -I"../" $(CCFLAGS) -Wno-psabi -o $@ -c $^

obj/loadLib.o: src/loadLib.cpp bin/dynamicLib.$(LIBEXT)
	g++ -I"../" $(CCFLAGS) -Wno-psabi -o $@ -c $<

bin/dynamicLib.$(LIBEXT): src/dynamicLib.cpp $(STATICLIB)
	g++ -I"../" $(CCFLAGS) -shared -Wl,--gc-sections -o $@ $^

bin/%: obj/%.o $(STATICLIB)
	g++ -o $@ $^ $(LFLAGS)

clean:
	$(RM) bin obj

.PHONY: all clean
