#include "split.hpp"

namespace z {
namespace core {

#define GENERATE \
	SPECIALIZE(ascii) \
	SPECIALIZE(utf8) \
	SPECIALIZE(utf16) \
	SPECIALIZE(utf32)

#define SPECIALIZE(ENCODING) \
	template <> \
	array<string<ENCODING>> split(const string<ENCODING> &input, const string<ENCODING> &delim = "") noexcept { \
		array<string<ENCODING>> list; \
\
		int beg = 0; \
		int len = delim.length(); \
\
		if (!len) { \
			for (auto i : input) { \
				list.add(string<ENCODING>(i)); \
			} \
			return list; \
		} \
\
		int end = input.findAfter(delim, beg); \
		while (end >= 0) { \
			list.add(input.substr(beg, end - beg)); \
\
			beg = end + len; \
			end = input.findAfter(delim, beg); \
		} \
\
		list.add(input.substr(beg, input.length() - beg)); \
\
		return list; \
	}

} // namespace core
} // namespace z