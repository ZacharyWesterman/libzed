If you want to add support for more operating systems, or any new features, bugfixes, etc. you can submit a merge request. If you have any questions you can email me at westerman.zachary@gmail.com

|Coding style / rules|
|:-----|
|Indent is a single *hard* tab. Only deviants use spaces.|
|All public entities must use `camelCaseLikeThis`. Please use reasonable and descriptive names.|
|All non-static functions, classes, enums, etc. must be under an appropriate namespace, with that under the `z` namespace.|
|Generally, the namespace hierarchy should exactly match the directory structure. E.g. the `z::core::string` class is in `z/core/string.hpp`.|
|I use [Doxygen](http://www.doxygen.org/) to document my code. All public classes, structs, enums, functions, etc. should have some documentation.|
|Private or static entities do not *have to* have any consistency or documentation.|
|I use [Catch2](https://github.com/catchorg/Catch2) for unit testing. If you add new features, make sure to write tests for them! If you want to write tests for existing code, please do!|

If you do submit changes, please follow my code style. No spaghetti code please, reading it physically hurts me.
