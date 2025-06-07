# Libzed

![build-linux](https://github.com/ZacharyWesterman/libzed/actions/workflows/build-linux.yml/badge.svg)
![build-windows](https://github.com/ZacharyWesterman/libzed/actions/workflows/build-windows.yml/badge.svg)
![tests](https://github.com/ZacharyWesterman/libzed/actions/workflows/tests.yml/badge.svg)

This library contains useful classes and functions for quick and simple data manipulation.
The idea is that this will not only take the grunt work out of C++, but it will help decrease
development time of other projects.

In particular, the idea behind rolling a custom string class was to have better built-in
support for string manipulation, as well as full support for different string formats.

Currently only Linux and Windows builds are supported, as those are the only machines I own.
Both 64 and 32-bit builds should be fully supported.
This also does compile on the Raspberry Pi and seems stable, however, tests currently do not run on that platform.

---

### Examples

Strings are chosen by encoding, not character size. In this way, they're consistent across operating systems.
They can be implicitly converted between encoding and are done so on-the-fly when interacting with streams.
```cpp
#include <z/core/string.hpp>
#include <iostream>

int main() {
	auto string1 = "Hello World!"_u32; //A UTF-32 encoded string.
	z::core::string<z::ascii> string2 = string1; //The same string, but in ASCII.

	std::cout << string1 << " : string1 is " << string1.size() << " bytes." << std::endl;
	std::cout << string2 << " : string2 is " << string2.size() << " bytes." << std::endl;

	auto string3 = "test1"_zs; //The default string encoding (UTF-8)
	zstring string4 = "test2"; //Ditto.
}
```
Strings also have a [full suite of operations][strings] that can be done on them, and can be cast to and from many types, including std::complex and floats in scientific notation.

---

When dealing with data that may have an unknown type at compile-time, a [generic datatype][generics] may come in handy.
```cpp
#include <z/util/generic.hpp>
#include <iostream>

using z::util::generic;

int main() {
	generic val1 = "Hello World!"; //string
	generic val2 = 23; //integer
	generic val3 = 3.14 //floating point
	generic val4 = { "Hello!", 10, 123.45 }; //Array of values of different types!

	std::cout << "val1 = " << val1.toString() << std::endl;
	std::cout << "val2 = " << val2.toString() << std::endl;
	std::cout << "val3 = " << val3.toString() << std::endl;

	//By default, arrays do not convert to a string (they may be very large!)
	std::cout << "val4 (default)   = " << val4.toString() << std::endl;
	//But we can specify that we want to recurse through any arrays.
	std::cout << "val4 (recursive) = " << val4.toString(true) << std::endl;
}
```

---

Another powerful feature is the support for various [generators][generators], with many useful built-ins and support for easy custom generators.
For example, a memory efficient way to read lines in a file is with `z::file::lines`:
```cpp
#include <z/file/lines.hpp>
#include <iostream>

using z::file::lines;

int main() {
	//Get the first 5 lines
	for (auto line : lines("your/file/here"_zs).limit(5)) {
		line.writeln(std::cout);
	}
}
```

Map, filter, and reduce are of course supported, which makes building custom generators extremely easy.
```cpp
#include <z/core/range.hpp>
#include <iostream>

using z::core::range;

int main() {
	//Generator for getting 1/n^2. See how the range goes from 1 to infinity, so this generator can be used forever.
	auto reciprocalSquares = range(1, z::core::infinity).map<double>([](auto n) { return 1.0 / (n * n); });

	//Generator for getting the max value from a list.
	auto items = generatorFrom({1, 10, 5, 42, 6, 3});
	auto max = items.reduce([](auto a, auto b) { return a > b ? a : b; });
}
```

There are **MANY** more features of generators, so be sure and check out the examples for more use cases!

---

There are a good handful of functions for interacting with files, all of which can be found [in the z::file namespace][file].

This includes a class for dynamically loading shared libraries at runtime ([documentation is here][libraries])!
As with the rest of this library, all the platform-specific stuff is abstracted away.

Check out the [examples/ directory][examples] for a longer list of example programs.

---

### List of structures

- z::core - Contains core functionality such as strings, arrays, and timers.
- z::file - Has functions for easy file system management.
- z::math - Mathematical constants and functions are defined here.
- z::util - Contains classes that provide more complicated functionality.
- z::system - Contains classes that allow interaction with and information about the user's system.

---

## Using This Library

Currently the Makefile supports compiling on Linux and Windows only, as those are the only systems I have. ¯\\\_(ツ)\_/¯

It should compile on any 64 or 32-bit flavor of these systems (including the Raspberry Pi!).

The only requirements are Make and a C++ compiler that supports *at least* C++11 (C++17 or newer preferred).
If `g++ --version` is 5 or above, chances are you're good.

### Required dependencies
- g++ (or other compliant compiler) supporting at least C++11
- make

### Optional dependency
This library has optional support for serialization via [cereal](https://github.com/USCiLab/cereal).
The following classes should be able to be saved and loaded with cereal:
- z::core::string
- z::core::array
- z::core::timer
- z::core::timeout
- z::util::dictionary
- z::util::generic

If cereal is not installed however, this library should compile just fine. Again, totally optional.

### Build options
By default, make should pick the best options for your machine. If that's not the case,
or you want/need different options for any other reason, here are some example flags you can tweak:
- `CC=c++` to compile with `c++` instead of `g++`.
- `LN=gcc` to link with `gcc` instead of `g++`.
- `BITS=64` to target 64-bit architecture instead of that of the host machine. Available options are 64, 32, or null.
- `STD=c++11` to use the C++11 standard instead of the default C++17. This is the same as the `-std=...` flag.
- `OPT=X` to set the optimization level. X should be a non-negative integer, (`s` or `size`), or (`f` or `fast`).
- `LIBDIR=/lib/install/dir` to set where the library should install under instead of `/usr/lib`.
- `ICLDIR=/include/install/dir` to set where the headers should be copied to instead of `/usr/include`.

### Compiling
Just run make in the project directory. I do the following:
```
$ make -j$(nproc)
```
so that it will compile on as many cores as possible.
On Windows you may have to run `mingw32-make` instead,
or if you are crosspiling for Windows on Linux, use `make TARGET=win64 -j$(nproc)` or `make TARGET=win32 -j$(nproc)`.
(Make sure to run `make clean` if you change the value of `TARGET`!!)

The above will compile as a dynamic library (.so on Linux, .dll on Windows).
To compile to a static library:
```
$ make -j$(nproc) static
```

You should get no errors or warnings.
You can open an issue on [GitHub][repo] if either of those pop up while compiling.

### Installing the dynamic library
In Linux, either run `make install` as superuser or `sudo make install`.

In Windows you can save the DLL into whatever directory you want,
just make sure the PATH system variable contains that folder.

Alternatively, you can copy the DLL/SO into the working directory of your project
and tell the linker to load it from there.

### Linking to this library
Suppose you have some simple project called `example.cpp`.
Let's have this file contain the following:
```cpp
#include <z/core/string.hpp>

int main()
{
	zstring string = "Hello world!";
	std::cout << string << std::endl;
	return 0;
}
```

To compile and link to the installed library:
```
$ g++ -std=c++17 -Wno-psabi -fPIC -o example.o -c example.cpp
$ g++ -lzed -o example.out example.o
```

If the DLL/SO and header files are saved in a specific directory that's not in PATH (for example if compiling for Windows):
```
$ g++ -I"/path/to/libzed/headers" -std=c++17 -Wno-psabi -fPIC -o example.o -c example.cpp
$ g++ -o example.out example.o -lzed -L"/path/to/lib"
```

If linking to the static library (libzed.a), I'd suggest removing unused code to cut down on resultant binary size.
The compile step for this is the same, just alter the linker step:
```
$ g++ -o example.out example.o -lzed -L"/path/to/lib" -Wl,--gc-sections
```
Note the effectiveness of this depends on the architecture and the specific compiler used.

Afterwards, if binary size is still an issue, stripping out symbols may help a bit.
You may also be able to use `gzexe` to compress the binary and reduce size even further.
Note that these tricks will **NOT** work with dll/so files, only executables!
```
$ strip example.out --strip-all
$ gzexe example.out && rm example.out~
```

---

[Download or fork this repo][repo] |
[Documentation is here][docs] |
[Look here if you want to contribute](CONTRIBUTING.md)

[docs]: https://zacharywesterman.github.io/libzed
[repo]: https://github.com/ZacharyWesterman/libzed
[strings]: https://zacharywesterman.github.io/libzed/classz_1_1core_1_1string.html
[generics]: https://zacharywesterman.github.io/libzed/classz_1_1util_1_1generic.html
[file]: https://zacharywesterman.github.io/libzed/namespacez_1_1file.html
[libraries]: https://zacharywesterman.github.io/libzed/classz_1_1file_1_1library.html
[examples]: https://zacharywesterman.github.io/libzed/examples.html
[generators]: https://zacharywesterman.github.io/libzed/classz_1_1core_1_1generator.html
