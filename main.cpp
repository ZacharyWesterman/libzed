#include <iostream>

//#define Z_USE_DOUBLE

#include <z/core.h>
#include <z/math.h>
#include <z/file.h>

using namespace std;
using namespace z;


int main()
{
    core::string<char> a = "ff.ff";

    cout << a.complexValue(16) << endl;

    //cout << (char)46 << endl;

    return 0;
}
