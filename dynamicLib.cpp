#include <iostream>

//g++ -shared -fPIC -o lib.so dynamicLib.cpp

extern "C"
{
    void f1()
    {
        std::cout << "In Dynamic Library.\n";
    }
}

