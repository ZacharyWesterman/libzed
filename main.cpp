#include <iostream>

//#include <z/core.h>
#include <z/math.h>
//#include <z/file.h>


using namespace std;
using namespace z;

int main()
{
    //complex<double> value;
    //cout << math::remainder(10, 8) << endl;
    cout << math::remainder(complex<long>(100,0), complex<long>(80, 0)) << endl;

    return 0;
}
