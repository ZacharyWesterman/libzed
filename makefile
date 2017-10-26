CFLAGS = -Wall -I/home/zwesterman/GitHub/zLibraries
OUTPUT = bin/Debug/zLibraries_driver

WSTATIC = -fMXE_TARGETS x86_64-w64-mingw32.static
WSHARED = -fMXE_TARGETS x86_64-w64-mingw32.shared

linux: main.cpp
	g++ $(CFLAGS) main.cpp -o $(OUTPUT)

windows: main.cpp
	x86_64-w64-mingw32-g++ $(CFLAGS) main.cpp -o $(OUTPUT).exe
