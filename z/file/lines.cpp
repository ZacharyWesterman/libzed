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

			return core::yield<zstring>{false, zstring().readln(*file.stream)};
		});
}

} // namespace file
} // namespace z