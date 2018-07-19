Strings Examples
----------------------


## Hello World

~~~cpp
#include <z/core.h>
#include <z/system.h>

using z::cotruere::string;
using z::system::console;

int main()
{
    string<z::utf8> foo = "Hello world!";
    console bar;

	foo.writeln(bar);

    return 0;
}
~~~

**Output:**
~~~
Hello world!
~~~

-----------------------------
## Uppercase vs. Lowercase

~~~cpp
#include <z/core.h>
#include <z/system.h>

using z::core::string;
using z::system::console;

int main()
{
    string<z::utf8> foo = "aN eXAMple StRiNg";
	console bar;

    foo.writeln(bar);
    foo.upper().writeln(bar);
    foo.lower().writeln(bar);
	foo.camel().writeln(bar);

    return 0;
}
~~~

**Output:**
~~~
aN eXAMple StRiNg
AN EXAMPLE STRING
an example string
An Example String
~~~
