#pragma once

#include "../core/string.hpp"
#include "exceptions.hpp"
#include <fstream>

namespace z {
namespace file {

void write(const zstring &contents, const zpath &filename) {
	std::ofstream file(filename.cstring());

	if (!file) {
		throw unwritable(filename);
	}

	contents.write(file);
}

} // namespace file
} // namespace z