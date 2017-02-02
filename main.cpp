#include <iostream>
#include "z/core/string/string.h"

using namespace std;
using namespace z;

int main()
{
    core::string<wchar_t> wide = L"Wide chars";

    core::string<char> narr = "Narrow chars";

    wide = narr;
    narr = "bleh";

    narr = wide;

    cout << narr.str() << endl;

    return 0;
}
