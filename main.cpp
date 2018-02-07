//#include <iostream>

//#define Z_USE_DOUBLE

#include <z/core.h>
#include <z/file.h>

using namespace z;


int main()
{
    core::console console;

    console.write(file::workingDir());

    return 0;
}



