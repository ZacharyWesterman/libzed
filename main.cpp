#include <iostream>

//#define Z_USE_DOUBLE

#include <z/core.h>
#include <z/math.h>
#include <z/file.h>

using namespace std;
using namespace z;
/*
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
*/
int main()
{
    core::generic foo = core::string<Char>("hello ");
    core::generic bar = Int(4);

    cout << foo.string().narrow().str() << endl;
    cout << bar.string().narrow().str() << endl;
    cout << (foo + bar).string().narrow().str() << endl;
    cout << (foo - bar).string().narrow().str() << endl;

    return 0;
}
