# Introduction

This collection of header-only C++ libraries contains useful classes and functions that help decrease development time of other projects.  
All code is under the **z** namespace. Currently, there are four other namespaces below that:<br>

**z::core**  
**z::file**  
**z::math**  
**z::util**  

All functions, classes and definitions are in one of these namespaces.  
The only exceptions are the three basic type definitions **Char**, **Int** and **Float** that are in the **z** namespace.

--------------------------
## z::core
Contains core functionality such as strings, arrays, and timers. No dependencies apart from stl.

## z::file
Has several classes for easy file system management. Depends on **z::core**.

## z::math
Mathematical constants and functions are defined here. No dependencies apart from stl.

## z::util
So far, only contains the **z::util::generic** class. Depends on **z::core** and **z::math**.
  
--------------------------
Sample programs and full code documentation available at [ZacharyWesterman.github.io](https://ZacharyWesterman.github.io).
