//#include <iostream>

//#define Z_USE_DOUBLE

#include <z/core.h>
#include <z/file.h>
#include <z/system.h>

using namespace z;



int main()
{
    core::stringStream<char> console("hello, this is a test. beep!");

    std::cout << (console.read()).str() << std::endl;

    /*system::cpuid cpu;



    console.write(cs("CPU Vendor: ") + cpu.vendor());
    console.write(cs("\nCPUs: ") + cpu.cpus());
    console.write(cs("\nCores: ") + cpu.cores());*/

    return 0;
}



