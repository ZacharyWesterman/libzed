# Strings Examples
----------------------


## Hello World

```cpp
#include <iostream>
#include <z/core.h>

using namespace std;
using namespace z;

int main()
{
    core::string<char> foo = "Hello world!";

    cout << foo.str() << endl;

    return 0;
}
```

**Output:**
```
Hello world!
```

-----------------------------
## Uppercase vs. Lowercase

```cpp
#include <iostream>
#include <z/core.h>

using namespace std;
using namespace z;

int main()
{
    core::string<char> foo = "aN eXAMple StRiNg";

    core::string<char> upper = foo.upper();
    core::string<char> lower = foo.lower();

    cout << foo.str() << endl;
    cout << upper.str() << endl;
    cout << lower.str() << endl;

    return 0;
}
```

**Output:**
```
aN eXAMple StRiNg
AN EXAMPLE STRING
an example string
```
