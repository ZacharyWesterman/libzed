//#include <iostream>

//#define Z_USE_DOUBLE

#include <z/core.h>
#include <z/file.h>
#include <z/system.h>

using namespace z;



int main()
{
    file::outputStream<char> output("test.txt");
    //core::stringStream<char> input("hello, this is a test.\nbeep!");
    //system::console input;

    output.write("test");

    /*system::cpuid cpu;



    console.write(cs("CPU Vendor: ") + cpu.vendor());
    console.write(cs("\nCPUs: ") + cpu.cpus());
    console.write(cs("\nCores: ") + cpu.cores());*/

    return 0;
}



