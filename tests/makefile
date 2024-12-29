
CC = g++
LN = g++
CFLAGS = --std=c++17 --coverage -I..
LFLAGS = --coverage

SRCS = $(wildcard $(addsuffix *.cpp, src/))
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

all: bin/libtest.so bin/run_tests

bin/run_tests: $(OBJS) ../libzed.a
	$(LN) -o $@ $^ $(LFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

bin/%.so: data/%.cpp
	$(CC) -I"../" -std=c++17 -shared -fPIC -Wl,--gc-sections -o $@ $^

clean:
	rm -f bin/* src/*.o src/*.gcda src/*.gcno *.gcov data/*.so

.PHONY: all clean
