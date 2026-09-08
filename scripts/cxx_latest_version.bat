@echo off
setlocal EnableDelayedExpansion

set "CC=%~1"
set "LAST="

for /f "delims=" %%i in ('"%CC%" -v --help 2^>nul') do (
    set "LINE=%%i"

    for %%j in (!LINE!) do (
        set "ARG=%%j"

        echo !ARG! | findstr /r /x "-std=c++[1-8][0-9]" >nul && (
            set "LAST=!ARG:-std=!"
        )
    )
)

echo %LAST%
