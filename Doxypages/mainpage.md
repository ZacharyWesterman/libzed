/** \mainpage Introduction

This library contains useful classes and functions for quick and simple data manipulation.
The idea is that this will help decrease development time of other projects.

Currently only Linux and Windows builds are supported, as those are the only machines I own.<br/>
I did also get it to compile on a Raspberry Pi (running Raspbian Stretch),
but I am not sure of any bugs that exist.
Should behave the same as on any other Linux system though.
___

- z::core
	Contains core functionality such as strings, arrays, and timers.

- z::file
	Has several classes for easy file system management.

- z::math
	Mathematical constants and functions are defined here.

- z::util
	Contains classes that provide more complicated functionality.

- z::system
	Contains classes that allow interaction with and information about the user's system.

___

##Using This Library

###Dependencies
- g++
- make

###Compiling
Just run make in the project directory. I do
~~~
$ make -j32
~~~

so that make will compile on as many cores as possible.

You should get no errors and only one warning (in library.cpp).
Open an issue on [GitHub][repo] if any other warnings pop up while compiling.

###Installing
In Linux, run the following as superuser:
~~~
$ make install
~~~

In Windows, you can save the DLL into whatever directory you want.
Just make sure the PATH system variable contains that folder.

Alternatively, you can copy the DLL/SO into the working directory of your project
and tell g++ to load it from there.

###Linking to this library
Suppose you have some simple project called `example.cpp`.
Let's have this file contain the following:
~~~{.cpp}
#include <z/core.h>
#include <z/system.h>

int main()
{
	z::system::console console;
	zstring string = "Hello world!";

	string.writeln(console);

	return 0;
}
~~~
Also let's assume you have libzed's .h files saved in the `/home/me/libzed` directory.
This directory will be different in Windows, but the commands will be the same.

To compile and link to the installed library:
~~~
$ g++ -I"/home/me/libzed" -std=c++11 -o example.o -c example.cpp
$ g++ -lzed -o example example.o
~~~

To compile and link to the DLL/SO saved in the application's working directory:
~~~
$ g++ -I"/home/me/libzed" -std=c++11 -o example.o -c example.cpp
$ g++ -lzed -L. -o example example.o
~~~

___
Download or fork: [GitHub repo][repo].

[repo]: https://github.com/ZacharyWesterman/zLibraries
