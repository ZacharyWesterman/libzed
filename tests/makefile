
CC = g++
LN = g++
STD = c++17
CFLAGS = -std=$(STD) --coverage -I..
LFLAGS = --coverage

SRCS = $(wildcard $(addsuffix *.cpp, src/))
OBJS = $(patsubst %.cpp,%.o,$(SRCS)) src/catch/catch_amalgamated.o
COVS := $(patsubst %,%.gcov,$(SRCS))

all: bin/run_tests

bin/run_tests: $(OBJS) ../libzed.a bin/libtest.so
	$(LN) -o $@ $^ $(LFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

bin/%.so: data/%.cpp
	$(CC) -I"../" -std=$(STD) -shared -fPIC -Wl,--gc-sections -o $@ $^

clean:
	rm -f bin/* src/*.o src/*.gcda src/*.gcno *.gcov data/*.so src/catch/*.o src/catch/*.gcda src/catch/*.gcno src/*.gcov coverage.*

pristine: clean
	rm -rf venv

coverage: coverage.txt
coverage.txt: $(COVS) venv/bin/gcovr
	( \
		. venv/bin/activate; \
		gcovr -r .. src --exclude src/catch --exclude src --txt $@ \
	)
coverage.html: $(COVS) venv/bin/gcovr
	( \
		. venv/bin/activate; \
		gcovr -r .. src --exclude src/catch --exclude src --html $@ \
	)

%.gcov: % makefile src/array.gcda
	gcov $< -t >$@

src/array.gcda: bin/run_tests
	bin/run_tests

venv/bin/gcovr: venv/bin/activate
	( \
		. venv/bin/activate; \
		python3 -m pip install gcovr \
	)

venv/bin/activate:
	python3 -m venv venv


.PHONY: all clean coverage pristine
