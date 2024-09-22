#include <iostream>
#include <z/system/cpuid.hpp>

int main() {
	z::system::cpuid hardware;

	("CPU vendor:    "_zs + hardware.vendor()).writeln(std::cout);
	("Thread count:  "_zs + hardware.cpus()).writeln(std::cout);
	("Logical cores: "_zs + hardware.cores()).writeln(std::cout);

	return 0;
}