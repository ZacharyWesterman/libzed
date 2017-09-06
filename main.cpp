#include <iostream>

//#define Z_USE_DOUBLE

//#include <z/core.h>
#include <z/math.h>
#include <z/file.h>

using namespace std;
using namespace z;

int main()
{
    file::reader<char> f;

    f.setFileName()

    f.read();

    cout << f.getContents() << endl;

    return 0;
}
