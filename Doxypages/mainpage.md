\mainpage zLibraries Documentation

## Introduction

This collection of header-only C++ libraries contains useful classes and functions that help decrease development time of other projects.  
All code is under the **z** namespace. Currently, there are four other namespaces below that:<br>

z::core  
z::file  
z::math  
z::util  
<br>
All functions, classes and definitions are in one of these namespaces.  
The only exceptions are the three basic type definitions z::Char, z::Int and z::Float.

--------------------------
## z::core
Contains core functionality such as strings, arrays, and timers. No dependencies apart from stl.

## z::file
Has several classes for easy file system management. Depends on z::core.

## z::math
Mathematical constants and functions are defined here. No dependencies apart from stl.

## z::util
So far, only contains the z::util::generic class. Depends on z::core and z::math.

--------------------------
Download or fork: [GitHub repo](https://github.com/ZacharyWesterman/zLibraries).  
Review the [License](md_Doxypages_LICENSE.html).