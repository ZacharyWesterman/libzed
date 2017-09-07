#include <iostream>

//#define Z_USE_DOUBLE

#include <z/core.h>
#include <z/math.h>
#include <z/file.h>

using namespace std;
using namespace z;

int main()
{
    core::timeout t;

    core::pause(360);

    cout << t.hours() << endl;

    return 0;
}
