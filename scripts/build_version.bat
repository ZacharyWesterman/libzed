@echo off

set "OUTPUT_FILE=%~1"
set "LIBZED_VERSION=%~2"
set "CXX_VERSION=%~3"

(
    echo #include "version.hpp"
    echo namespace z {
    echo     const char *const VERSION = "%LIBZED_VERSION%";
    echo     const char *const CXX_VERSION = "%CXX_VERSION%";
    echo }
) > "%OUTPUT_FILE%"
