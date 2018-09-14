[![Build Status](https://travis-ci.org/ZacharyWesterman/libzed.svg?branch=master)](https://travis-ci.org/ZacharyWesterman/libzed)

To link against this library:
(Assuming your program is called "driver")

```
g++ -L. -lzed -Wl,-rpath=. -o driver driver.cpp
```

TODO: add more to readme. A lot has changed since it was last updated.
