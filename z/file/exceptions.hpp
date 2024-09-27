#pragma once

#include "../core/string.hpp"

namespace z {
namespace file {

/**
 * @class unreadable
 * @brief Exception thrown when a file cannot be opened for reading.
 */
class unreadable : public std::runtime_error {
public:
	/**
	 * @brief Constructor.
	 * @param filename The path of the file that could not be opened.
	 */
	unreadable(const zpath &filename) : std::runtime_error(("Unable to open "_zs + filename + " for reading.").cstring()) {}
};

/**
 * @class unwritable
 * @brief Exception thrown when a file cannot be opened for writing.
 */
class unwritable : public std::runtime_error {
public:
	/**
	 * @brief Constructor.
	 * @param filename The path of the file that could not be opened.
	 */
	unwritable(const zpath &filename) : std::runtime_error(("Unable to open "_zs + filename + " for writing.").cstring()) {}
};

} // namespace file
} // namespace z