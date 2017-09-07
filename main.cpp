#include <iostream>

//#define Z_USE_DOUBLE

//#include <z/core.h>
#include <z/math.h>
#include <z/file.h>

using namespace std;
using namespace z;

int main()
{
    file::writer<wchar_t> w ("test.txt", L"שלום, באסקר");

    core::timeout t (0);

    do
    {
        cout << '.';
    } while(!w.write(t));


    return 0;
}
