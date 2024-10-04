#include "lines.hpp"
#include "exceptions.hpp"

namespace z {
namespace file {

fileHandle::~fileHandle() {
	if (used) {
		delete stream;
	}
}

core::generator<zstring, fileHandle> lines(const zpath &filename) {
	return core::generator<zstring, fileHandle>(
		fileHandle{
			filename,
			new std::ifstream(filename.cstring()),
			false,
		},
		[](fileHandle &file) {
			file.used = true;

			if (file.stream->eof()) {
				return core::yield<zstring>{true, ""};
			}

			if (file.stream->fail()) {
				throw unreadable(file.filename);
			}

			const auto result = zstring().readln(*file.stream);

			return core::yield<zstring>{
				(!result.length() && file.stream->eof()), // If a file ends with a line ending, ignore that last one.
				result,
			};
		});
}

core::generator<zstring, fileHandle> lines(std::istream &stream) noexcept {
	return core::generator<zstring, fileHandle>(
		fileHandle{
			"<istream>",
			&stream,
			false,
		},
		[](fileHandle &file) {
			if (file.stream->eof() || file.stream->fail()) {
				return core::yield<zstring>{true, ""};
			}

			const auto result = zstring().readln(*file.stream);

			return core::yield<zstring>{
				(!result.length() && file.stream->eof()), // If a stream ends with a line ending, ignore that last one.
				result,
			};
		});
}

} // namespace file
} // namespace z