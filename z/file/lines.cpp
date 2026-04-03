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
		[](fileHandle &file) -> std::optional<zstring> {
			file.used = true;

			if (file.stream->eof()) {
				return {};
			}

			if (file.stream->fail()) {
				throw unreadable(file.filename);
			}

			const auto result = zstring().readln(*file.stream);

			// If a stream ends with a line ending, ignore that last one.
			if (!result.length() && file.stream->eof()) {
				return {};
			}

			return result;
		});
}

core::generator<zstring, fileHandle> lines(std::istream &stream) noexcept {
	return core::generator<zstring, fileHandle>(
		fileHandle{
			"<istream>",
			&stream,
			false,
		},
		[](fileHandle &file) -> std::optional<zstring> {
			if (file.stream->eof() || file.stream->fail()) {
				return {};
			}

			const auto result = zstring().readln(*file.stream);

			// If a stream ends with a line ending, ignore that last one.
			if (!result.length() && file.stream->eof()) {
				return {};
			}

			return result;
		});
}

} // namespace file
} // namespace z
