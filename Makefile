all: lib exe

lib:
	g++ -shared -fPIC -o libtest.so dynamicLib.cpp

exe:
	g++ -o loadLib loadLib.cpp -ldl

clean:
	rm -f loadLib libtest.so.1.0 dynamicLib.o libtest.so
