#pragma once

#include "../core/generator.hpp"
#include "../core/string.hpp"
#include <fstream>

namespace z {
namespace file {

/**
 * @brief State management struct for file streaming generator.
 */
struct fileHandle {
	/// The filename.
	zpath filename;
	/// The current stream.
	std::istream *stream;
	/// Whether the file has been opened yet.
	bool used;
	/// Destructor.
	~fileHandle();
};

/**
 * @brief Reads the contents of a file line-by-line.
 *
 * This function uses a generator to read files, which can be particularly useful with
 * files that may be too large to load all at once.
 *
 * @param filename The path and name of the file to read from.
 *
 * @throws z::file::unreadable If the file cannot be opened or read from.
 */
core::generator<zstring, fileHandle> lines(const zpath &filename);

/**
 * @brief Reads the contents of a stream line-by-line.
 *
 * This function uses a generator to read from an arbitrary stream,
 * which may or may not be a file.
 *
 * @param stream The input stream to read from.
 */
core::generator<zstring, fileHandle> lines(std::istream &stream) noexcept;

} // namespace file
} // namespace z