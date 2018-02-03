#include <iostream>

//#define Z_USE_DOUBLE

#include <z/core.h>
#include <z/util.h>

using namespace std;
using namespace z;

class output : public core::stream<char>
{
public:
    virtual void putChar(char c) {cout << c;}

    virtual char getChar() {return 0;}

    virtual output& operator<<(const core::string<char>& input)
    {
        cout << input.str();

        return *this;
    }

    virtual output& operator>>(core::string<char>& output) {return *this;}

    virtual bool eof()
    {
        return 0;
    }
};

void print(core::stream<char>& stream)
{
    stream.putChar('a');
}

int main()
{
    output out;

    print (out);

    return 0;
}



