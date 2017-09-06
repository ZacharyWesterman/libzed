#include <iostream>

//#define Z_USE_DOUBLE

//#include <z/core.h>
#include <z/math.h>
//#include <z/file.h>
#include <stdint.h>

using namespace std;
using namespace z;

int main()
{
    for (uint64_t i=0; i<65; i++)
    {
        cout << "f(" << i << ") =\t";
        cout << Float(math::factorial(i)) << endl;
    }

    //cout << endl << sizeof(int8_t) << endl;

    return 0;
}
