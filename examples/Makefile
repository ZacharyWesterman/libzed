
ifeq ($(OS),Windows_NT)
LFLAGS = -L. -lzed
DLL1 = zed.dll
DLL2 = binary/zed.dll
else
LFLAGS = -lzed
endif

ifeq (, $(shell rm --version))
RM = rd /s /q
MV = move
CP = copy
else
RM = rm -rf
MV = mv
CP = cp
endif

all: binary object $(DLL1) binary/regexConsole binary/fileStream $(DLL2)

binary:
	mkdir -p $@

object:
	mkdir -p $@

object/%.o: source/%.cpp
	g++ -I"../" -std=c++11 -fPIC -o $@ -c $^

binary/%: object/%.o
	g++ $(LFLAGS) -o $@ $^

binary/zed.dll: zed.dll
	$(MV) $^ $@

zed.dll: ../zed.dll
	$(CP) $^ $@

clean:
	$(RM) binary object

.PHONY: all clean
