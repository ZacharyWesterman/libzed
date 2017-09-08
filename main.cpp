#include <iostream>

//#define Z_USE_DOUBLE

#include <z/core.h>
#include <z/math.h>
#include <z/file.h>

using namespace std;
using namespace z;

core::timer t;

core::array<int> cpy (const core::array<int> x)
{

    core::array<int> y = x;
    t.reset();

    return y;
}

int main()
{
    core::array<int> b;
    for (int i=0; i<1000000; i++)
        b.add(i);


    core::array<int> a = cpy (b);
    cout << t.millis() << endl;

    //for (int i=0; i<a.size(); i++)
        //cout << a[i] << endl;

    return 0;
}
