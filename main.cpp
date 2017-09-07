#include <iostream>

//#define Z_USE_DOUBLE

//#include <z/core.h>
#include <z/math.h>
#include <z/file.h>

using namespace std;
using namespace z;

int main()
{
    core::array< core::string<char> > files;

    files = file::listFilesInDir("", "*");

    for (int i=0; i<files.size(); i++)
        cout << files[i].str() << endl;

    return 0;
}
