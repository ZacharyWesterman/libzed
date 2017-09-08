#include <iostream>

//#define Z_USE_DOUBLE

#include <z/core.h>
#include <z/math.h>
#include <z/file.h>

using namespace std;
using namespace z;


int main()
{
    core::sortedRefArray<int*> a {new int,new int,new int};

    a.add(new int);

    cout << "{";
    for (int i=0; i<a.size(); i++)
    {
        if (i)
            cout << ",";
        cout << a[i];
    }
    cout << "}\n";

    return 0;
}
