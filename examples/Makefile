
ifeq ($(OS),Windows_NT)
LFLAGS = -L. -lzed
DLL1 = zed.dll
DLL2 = binary/zed.dll
LIBEXT = dll
else
LFLAGS = -lzed
LIBEXT = so
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

STD = c++17

all: binary object $(DLL1) binary/regexConsole binary/fileStream binary/dynamicLib.$(LIBEXT) binary/loadLib $(DLL2) binary/generic

binary:
	mkdir $@

object:
	mkdir $@

object/%.o: source/%.cpp
	g++ -I"../" -std=$(STD) -fPIC -o $@ -c $^

binary/dynamicLib.$(LIBEXT): source/dynamicLib.cpp
	g++ -I"../" -std=$(STD) -shared -fPIC -o $@ $^

binary/%: object/%.o
	g++ $(LFLAGS) -o $@ $^

binary/zed.dll: zed.dll
	$(MV) "$^" "$@"

zed.dll: ../zed.dll
	$(CP) "$@"

clean:
	$(RM) binary object

.PHONY: all clean
