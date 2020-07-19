
ifeq ($(OS),Windows_NT)
LFLAGS = -L.. -lzed
else
LFLAGS = -lzed
endif

all: binary object binary/regexConsole binary/fileStream

binary:
	mkdir -p $@

object:
	mkdir -p $@

object/%.o: source/%.cpp
	g++ -I"../" -std=c++11 -fPIC -o $@ -c $^

binary/%: object/%.o
	g++ $(LFLAGS) -o $@ $^

clean:
	rm -rf binary object

.PHONY: all clean
