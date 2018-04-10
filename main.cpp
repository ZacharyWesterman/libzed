#include <z/system.h>
#include <z/util.h>
#include <z/core.h>
#include <z/file.h>

using namespace z;

int main()
{
	system::console console;

	util::genericData* var = new util::genericDataString("3+i");

	// console.writeln((var->complex()));
	std::cout<< var->complex() << std::endl;
	std::cout<< var->string().isComplex() << std::endl;

    return 0;
}
