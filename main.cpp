#include <iostream>

//#define Z_USE_DOUBLE

//#include <z/core.h>
#include <z/math.h>
//#include <z/file.h>


using namespace std;
using namespace z;

typedef unsigned long long ull;

int main()
{
    for (ull i=0; i<50; i++)
    {
        cout << "f(" << i << ") =\t";
        cout << math::factorial(i) << endl;
    }

    cout << endl << sizeof(ull) << endl;

    return 0;
}
