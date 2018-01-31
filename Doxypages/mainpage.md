## Introduction				{#mainpage}

This collection of header-only C++ libraries contains useful classes and functions for quick and simple data manipulation.  
The idea is that this will help decrease development time of other projects.  

--------------------------
All code is under the `z` namespace. Currently, there are four other namespaces below that:  

`z::core`  
`z::file`  
`z::math`  
`z::util`  
  
All functions, classes and definitions are in one of these namespaces.  
The only exceptions are the three basic type definitions z::Char, z::Int and z::Float.

--------------------------

## z::core
Contains core functionality such as strings, arrays, and timers. No dependencies apart from stl.

## Examples:
<table>
<tr>
<th>Core</th>
<th>File</th>
<th>Math</th>
<th>Util</th>
</tr>
<tr>
<td>[Strings](Doxypages/examples/core/string.md)</td>
<td>[Reader](#)</td>
<td></td>
<td></td>
</tr>
</table>

## z::file
Has several classes for easy file system management. Depends on z::core.

#### Example
```cpp

```

## z::math
Mathematical constants and functions are defined here. No dependencies apart from stl.

#### Example
```cpp

```

## z::util
So far, only contains the z::util::generic class. Depends on z::core and z::math.

#### Example
```cpp

```

--------------------------
Download or fork: [GitHub repo](https://github.com/ZacharyWesterman/zLibraries).  
Review the [License](md_Doxypages_LICENSE.html).