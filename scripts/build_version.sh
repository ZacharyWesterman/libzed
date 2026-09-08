#!/usr/bin/env sh

OUTPUT_FILE="$1"
LIBZED_VERSION="$2"
CXX_VERSION="$3"

echo "#include \"version.hpp\"
namespace z {
    const char *const VERSION = \"$LIBZED_VERSION\";
    const char *const CXX_VERSION = \"$CXX_VERSION\";
}" > "$OUTPUT_FILE"
