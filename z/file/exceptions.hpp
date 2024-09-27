#pragma once

#include "../core/string.hpp"

namespace z {
namespace file {

class unreadable : public std::runtime_error {
public:
	unreadable(const zpath &filename) : std::runtime_error(("Unable to open "_zs + filename + " for reading.").cstring()) {}
};

class unwritable : public std::runtime_error {
public:
	unwritable(const zpath &filename) : std::runtime_error(("Unable to open "_zs + filename + " for writing.").cstring()) {}
};

} // namespace file
} // namespace z