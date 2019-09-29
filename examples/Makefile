
all: binary object binary/regexConsole binary/fileStream

binary:
	mkdir -p $@

object:
	mkdir -p $@

object/%.o: source/%.cpp
	g++ -I"../" -std=c++11 -fPIC -o $@ -c $^

binary/%: object/%.o
	g++ -lzed -o $@ $^

clean:
	rm -rf binary object

.PHONY: all clean
