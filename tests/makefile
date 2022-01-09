
CC = g++
LN = g++
CFLAGS = --std=c++11
LFLAGS =

SRCS = $(wildcard $(addsuffix *.cpp, src/))
OBJS = $(patsubst src/%.cpp,obj/%.o,$(SRCS))

bin/run_tests: $(OBJS) ../libzed.a
	$(LN) -o $@ $^ $(LFLAGS)

obj/%.o: src/%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f bin/* obj/*

.PHONY: all clean
