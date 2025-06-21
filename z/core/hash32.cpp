#include "hash32.hpp"

namespace z {
namespace core {

hash32 crc32(const char *str, int size) noexcept {
	hash32 crc = 0xFFFFFFFF;
	for (int i = 0; i < size; ++i) {
		crc = (crc >> 8) ^ crc_table[(crc ^ str[i]) & 0xFF];
	}
	return crc ^ 0xFFFFFFFF;
}

} // namespace core
} // namespace z
