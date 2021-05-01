[![Build Status](https://travis-ci.com/ZacharyWesterman/libzed.svg?branch=master)](https://travis-ci.org/ZacharyWesterman/libzed)

This library contains useful classes and functions for quick and simple data manipulation. The idea is to take the grunt work out of C++ in a fast, platform-independent way.

### Examples

Strings are chosen by encoding, not character size. In this way, they're consistent across operating systems.
They can be implicitly converted between encoding and are done so on-the-fly when interacting with streams.
```c++
#include <z/core/string.hpp>
#include <iostream>

int main()
{
	auto string1 = "Hello World!"_u32; //A UTF-32 encoded string.
	z::core::string<z::ascii> string2 = string1; //The same string, but in ASCII.

	std::cout << string1 << " : string1 is " << string1.size() << " bytes." << std::endl;
	std::cout << string2 << " : string2 is " << string2.size() << " bytes." << std::endl;
}
```
Strings also have a [full suite of operations][strings] that can be done on them, and can be cast to and from many types, including std::complex and floats in scientific notation.

---

When dealing with data that may have an unknown type at compile-time, a [generic datatype][generics] may come in handy.
```c++
#include <z/util/generic.hpp>
#include <iostream>

int main()
{
	z::util::generic val1 = "Hello World!"; //string
	z::util::generic val2 = 23; //integer
	z::util::generic val3 = 3.14 //floating point
	z::util::generic val4 = { "Hello!", 10, 123.45 }; //Array of values of different types!

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

There are also functions for interacting with files, all of which can be found [in the z::file namespace][file].

On this note, and in the interest of simplifying things, there also exists a class for dynamically loading and handling libraries at runtime ([documentation is here][libraries]).</br>
See the examples/ folder for a small example of dynamic library loading.
As with the rest of this library, all the platform-specific stuff is abstracted away.

---

### Building This Library

Currently the Makefile supports compiling on Linux and Windows only, as those are the only systems I have. ¯\\\_(ツ)\_/¯<br>
It should compile on any 64 or 32-bit flavor of these systems (including the Raspberry Pi!).

The only requirements are Make and a C++ compiler that supports *at least* C++11 (C++17 or newer preferred).
If `g++ --version` is 5 or above, chances are you're good.

Full build instructions along with make options are detailed on the main page of the [Documentation][docs].

Also of note is that certain classes in this library can be serialized if the
[cereal](https://github.com/USCiLab/cereal) headers are available!
Just make sure they're in the include path and you should be all set.
This is optional, and not necessary to use this library.

[Documentation is here.][docs]

[Look here if you want to contribute.](CONTRIBUTING.md)

[docs]: https://zacharywesterman.github.io
[strings]: https://zacharywesterman.github.io/libzed/html/classz_1_1core_1_1string.html
[generics]: https://zacharywesterman.github.io/libzed/html/classz_1_1util_1_1generic.html
[file]: https://zacharywesterman.github.io/libzed/html/namespacez_1_1file.html
[libraries]: https://zacharywesterman.github.io/libzed/html/classz_1_1file_1_1library.html
