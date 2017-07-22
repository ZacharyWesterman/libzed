#include <iostream>

#include <z/core.h>
//#include <z/math.h>
//#include <z/file.h>


using namespace std;
using namespace z;

int main()
{
    //complex<double> value;
    //cout << math::remainder(10, 8) << endl;
    //cout << math::remainder(complex<long>(100,0), complex<long>(80, 0)) << endl;

    core::string<char> s = 1.0 * 100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.0;

    cout << s.str() << endl;
    cout << (int)'E' << endl;

    core::double_cast;

    return 0;
}
