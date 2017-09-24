#include <iostream>

//#define Z_USE_DOUBLE

#include <z/core.h>
#include <z/math.h>
#include <z/file.h>

using namespace std;
using namespace z;

typedef core::array<core::string<char> > strarray;

char toHex(char _c)
{
    char _h;

    if (_c < 10)
        _h = _c + '0';
    else
        _h = _c - 10 + 'A';

    return _h;
}

core::string<char> hex(unsigned char _c)
{
    unsigned char c_l = _c & 0b00001111;
    unsigned char c_h = _c >> 4;

    core::string<char> _ret = toHex(c_h);
    _ret += toHex(c_l);

    return _ret;
}

int main()
{
    /*file::library lib;

    lib.load("./libtest.so");

    void (*f1)();

    f1 = (void (*)())lib.symbol("f1");

    f1();*/

    core::string<char> s = "hello";

    cout << s.substr(4,0).str() << endl;

    /*strarray A =
    {
        "a", "b", "c", "d", "e"
    };

    strarray B = A.subset(4,0);

    cout << "{";
    for (int i=0; i<B.size(); i++)
    {
        if (i)
            cout << ",";
        cout << B[i].str();
    }
    cout << "}\n";*/

    return 0;
}
