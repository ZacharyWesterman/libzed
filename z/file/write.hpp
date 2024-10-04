#pragma once

#include "../core/string.hpp"
#include "exceptions.hpp"
#include <fstream>

namespace z {
namespace file {
/**
 * @brief Writes the contents of a string to a specified file.
 *
 * On success, the entire contents of the file will be overwritten.
 *
 * @param contents The contents to be written to the file.
 * @param filename The path and name of the file to write to.
 * @param append If true, appends to the file. Otherwise overwrites contents.
 *
 * @throws z::file::unwritable If the file cannot be opened or written to.
 */
void write(const zstring &contents, const zpath &filename, bool append = false);

} // namespace file
} // namespace z