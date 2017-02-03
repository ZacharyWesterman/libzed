#include <iostream>
#include "z/core/string/string.h"

using namespace std;
using namespace z;

int main()
{
    core::string<wchar_t> wide = 'a';

    core::string<char> narr = wide;

    cout << narr.str() << endl;

    return 0;
}
