
CC = g++
LN = g++
CFLAGS = --std=c++17 --coverage -I..
LFLAGS = --coverage

SRCS = $(wildcard $(addsuffix *.cpp, src/))
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

bin/run_tests: $(OBJS) ../libzed.a
	$(LN) -o $@ $^ $(LFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f bin/* src/*.o src/*.gcda src/*.gcno *.gcov

.PHONY: all clean
