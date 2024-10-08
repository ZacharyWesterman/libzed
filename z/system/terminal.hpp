#pragma once

#include <ostream>

namespace z {
namespace system {

/**
 * @brief A simple struct for storing terminal width and height.
 */
struct termsize {
	/// The terminal width.
	int width;
	/// The terminal height.
	int height;
};

/**
 * @brief Get the current dimensions of the terminal.
 *
 * Gets the dimensions of the terminal for a specified stream.
 * E.g. `std::cout` or `std::cerr` can be passed to get the respective
 * terminal size for each. Any other stream defaults to 80x24.
 *
 * @param stream The stream to check terminal size of.
 * @return The width and height of the terminal.
 */
termsize terminal(const std::ostream &stream) noexcept;

} // namespace system
} // namespace z