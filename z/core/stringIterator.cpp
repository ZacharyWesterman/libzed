#include "stringIterator.hpp"
#include "charFunctions.hpp"

namespace z {
namespace core {
template <>
stringIterator<utf32>::stringIterator(uint8_t *ptr, size_t offset) noexcept {
	data = ptr + (offset << 2);
	auto tmp = (uint32_t *)data;
	chr = *tmp;
}

template <>
stringIterator<utf16>::stringIterator(uint8_t *ptr, size_t offset) noexcept {
	data = ptr + (offset << 1);
	auto tmp = (uint16_t *)data;
	chr = *tmp;
}

template <>
stringIterator<utf8>::stringIterator(uint8_t *ptr, size_t offset) noexcept {
	data = ptr + offset;
	chr = fromUTF8(data);
}

template <>
stringIterator<ascii>::stringIterator(uint8_t *ptr, size_t offset) noexcept {
	data = ptr + offset;
	chr = *data;
}

template <>
stringIterator<utf32> stringIterator<utf32>::operator++() noexcept {
	data += 4;
	auto tmp = (uint32_t *)data;
	chr = *tmp;
	return *this;
}

template <>
stringIterator<utf16> stringIterator<utf16>::operator++() noexcept {
	data += 2;
	auto tmp = (uint16_t *)data;
	chr = *tmp;
	return *this;
}

template <>
stringIterator<utf8> stringIterator<utf8>::operator++() noexcept {
	data += lenFromUTF8(data);
	chr = fromUTF8(data);
	return *this;
}

template <>
stringIterator<ascii> stringIterator<ascii>::operator++() noexcept {
	++data;
	chr = *data;
	return *this;
}

} // namespace core
} // namespace z
