#include <iostream>

//#define Z_USE_DOUBLE

#include <z/core.h>
#include <z/math.h>
//#include <z/file.h>


using namespace std;
using namespace z;

int main()
{
    core::string<wchar_t> s = L"שלום, זה טקסט: hello þ bepos";

    cout << core::string<char>(s).str() << endl;

    /*
    long total_micros = 0;

    core::timer time(true);

    int i = 1;
    for (i=1; i<=1000; i++)
    {
        time.reset();
        math::zeta(2);
        total_micros += time.micros();

        //if (i % 10)
            //cout << ".";
    }
    cout << endl;
    cout << "Average time to calculate Zeta function:\n"
         << ((zFloat)total_micros/(zFloat)i) / 1000.0 << " milliseconds\n";
    core::string<char> z (math::zeta(2));
    cout << z.str() << endl;*/

    return 0;
}
