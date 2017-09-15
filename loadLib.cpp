#include <iostream>
#include <dlfcn.h>

using namespace std;

//g++ -o loadLib loadLib.cpp -ldl

int main()
{
    char* err = 0;

    void* lib = dlopen("./libtest.so", RTLD_NOW);
    err = dlerror();

    if (err)
    {
        cout << err << endl;
        return -1;
    }

    void* proc = dlsym(lib, "f1");
    err = dlerror();

    if (err)
    {
        cout << err << endl;
        return -1;
    }


    cout << lib << " " << proc << endl;

    void (*f1)() = (void(*)())proc;

    f1();

    dlclose(lib);

    return 0;
}
