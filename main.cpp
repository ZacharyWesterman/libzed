#include <iostream>

#include <z/core.h>
#include <z/math.h>
#include <z/file.h>

using namespace std;
using namespace z;

int main()
{
    core::timeout time(16667); //timeout 1/60th of a second ~16667 micro seconds.

    for (int i=0; i<3340000; i++);

    cout << (time.timedOut() ? "Timed out" : "Did not time out") << endl;
    cout << (double)time.micros()/1000000 << "s\n";

    return 0;
}
