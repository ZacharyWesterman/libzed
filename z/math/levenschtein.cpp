#include "levenschtein.hpp"
#include <algorithm>
#include <limits>

namespace z {
namespace math {
#define GENERATE \
	SPECIALIZE(ascii) \
	SPECIALIZE(utf8) \
	SPECIALIZE(utf16) \
	SPECIALIZE(utf32)

#define SPECIALIZE(ENCODING) \
	template <> \
	int levenschtein(const z::core::string<ENCODING> &word1, const z::core::string<ENCODING> &word2, int max) noexcept { \
		const int len1 = word1.length(); \
		const int len2 = word2.length(); \
\
		auto row0 = new int[len2 + 1]; \
		auto row1 = new int[len2 + 1]; \
\
		for (int i = 0; i <= len2; ++i) { \
			row0[i] = i; \
		} \
\
		for (int i = 0; i <= len1 - 1; ++i) { \
			row1[0] = i + 1; \
\
			for (int j = 0; j <= len2 - 1; ++j) { \
				auto deletionCost = row0[j + 1] + 1; /*NOLINT*/ \
				auto insertionCost = row1[j] + 1; \
				auto substitutionCost = row0[j] + (word1[i] != word2[j]); \
\
				row1[j + 1] = std::min(std::min(deletionCost, insertionCost), substitutionCost); \
\
				/*If the current distance is greater than the max, just exit*/ \
				if (row1[j + 1] > max + 2) { \
					delete[] row0; \
					delete[] row1; \
					return std::numeric_limits<int>::max(); \
				} \
			} \
\
			/*Swap rows*/ \
			auto temp = row0; \
			row0 = row1; \
			row1 = temp; \
		} \
\
		auto result = row0[len2]; \
		delete[] row0; \
		delete[] row1; \
		return result; \
	}

GENERATE
} // namespace math
} // namespace z