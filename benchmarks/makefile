
CC = g++
LN = g++
STD = c++17
CFLAGS = -std=$(STD) --coverage -I..
LFLAGS = --coverage

SRCS = $(wildcard $(addsuffix *.cpp, src/))
OBJS = $(patsubst %.cpp,%.o,$(SRCS)) src/catch/catch_amalgamated.o

all: bin/run_benchmarks

bin/run_benchmarks: $(OBJS) ../libzed.a
	$(LN) -o $@ $^ $(LFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f bin/* src/*.o src/*.gcda src/*.gcno *.gcov data/*.so src/catch/*.o src/catch/*.gcda src/catch/*.gcno

.PHONY: all clean
