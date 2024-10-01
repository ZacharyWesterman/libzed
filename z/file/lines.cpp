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

} // namespace file
} // namespace z