#!/usr/bin/env bash

set -e

function error() {
    echo >&2 -e "\e[31mERROR:\e[0m $*"
    echo >&2
    echo >&2 "Note: On Linux, compiling for both Windows and Linux requires \`g++\` and \`x86_64-w64-mingw32-g++\` to be installed, with both supporting at least C++17"'!'
    exit 1
}

function info() {
    echo >&2 -e "\e[34mINFO:\e[0m $*"
}

if ! command -V g++ &>/dev/null; then
    error 'g++ not found, so cannot compile! Aborting.'
fi

if ! command -V x86_64-w64-mingw32-g++ &>/dev/null; then
    error 'x86_64-w64-mingw32-g++ not found, so cannot cross-compile for Windows. Aborting.'
fi

for CC in g++ x86_64-w64-mingw32-g++; do
    cxx_ver=$(scripts/cxx_latest_version.sh $CC | sed 's/[^0-9]//g')
    if [ "$cxx_ver" == '' ] || [ "$cxx_ver" -lt 17 ]; then
        error "($CC) Max C++ version is $cxx_ver, but at least 17 is required. Aborting."
    fi
done

rm -rf build
mkdir -p build

echo -n "
Linux:

- libzed.a (static)
- libzed.so (dynamic)

Windows:

- libzed-win.a (static)
- libzed.dll (dynamic)

All operating systems:

- libzed-headers.zip (dev interface)
" > build/release-notes.txt

info "Building for Windows..."
make clean &>/dev/null
make TARGET=win64 -j"$(nproc)" static dynamic &>/dev/null
mv libzed.a build/libzed-win.a
mv zed.dll build/

info "Building for Linux..."
make clean &>/dev/null
make TARGET=linux -j"$(nproc)" static dynamic headers &>/dev/null
mv libzed-headers.zip libzed.a build/
mv -- *.so build/libzed.so

info 'Done! Output files are stored in ./build/'
