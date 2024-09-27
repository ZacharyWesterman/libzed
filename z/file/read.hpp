#pragma once

#include "../core/string.hpp"
#include "exceptions.hpp"
#include <fstream>

namespace z {
namespace file {
/**
 * @brief Reads the entire contents of a specified file.
 *
 * @param filename The path and name of the file to read from.
 *
 * @throws z::file::unreadable If the file cannot be opened or read from.
 */
zstring read(const zpath &filename) {
	std::ifstream file(filename.cstring());

	if (!file) {
		throw unreadable(filename);
	}

	return zstring().readall(file).trimIn('\n');
}

} // namespace file
} // namespace z