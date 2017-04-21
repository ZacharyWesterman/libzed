#include <iostream>

#include <z/core.h>
#include <z/math.h>
#include <z/file.h>

using namespace std;
using namespace z;

int main()
{
    core::timer total;
    core::timeout time(100); //timeout at 100 micros

    file::writer<wchar_t> fwriter;
    fwriter.setFileName("test.txt");
    fwriter.setFileData(L"Հայաստան Россия Österreich Ελλάδα भारत");

    int iterations = 1;
    while (!fwriter.write(time))
    {
        iterations++;
        time.reset();
    }

    cout << "Writing required " << iterations\
         << " iteration" << ((iterations==1)?"":"s")\
         << " and took " << (double)total.micros()/1000000\
         << " seconds\n";

    //cout << "Contents:\n" << floader.getContents() << endl;

    return 0;
}
