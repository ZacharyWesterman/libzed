#pragma once

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
 * @return The width and height of the terminal.
 */
termsize terminal() noexcept;

} // namespace system
} // namespace z