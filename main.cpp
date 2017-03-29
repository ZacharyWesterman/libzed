#include <iostream>

#include <z/core.h>
#include <z/math.h>

using namespace std;
using namespace z;

int main()
{
    core::string<wchar_t> wide;

    math::matrix<3> mat;

    mat.value[0][0] = 1;
    mat.value[0][1] = 0;
    mat.value[1][0] = 0;
    mat.value[1][1] = 2;

    cout << mat.det() << endl;

    return 0;
}
