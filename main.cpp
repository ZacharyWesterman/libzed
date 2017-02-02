#include <iostream>
#include "z/core/string/string.h"

using namespace std;
using namespace z;

int main()
{
    core::string<wchar_t> wide = L"Wide chars";

    core::string<char> narr = "Narrow chars";

    narr = wide;

    return 0;
}
