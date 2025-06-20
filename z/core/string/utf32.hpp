#pragma once

#include <iostream>

namespace z {
namespace core {
template <>
void string<utf32>::increase(int max_chars) noexcept {
	int goal = (max_chars << 2) + 4; // account for null byte at the end
	if (data_len >= goal) {
		return;
	}

	uint8_t *old_data = data;
	int old_data_len = data_len;

	//~1.5x string growth
	while (data_len < goal) {
		data_len += (data_len + 4) >> 1;
	}
	data = new uint8_t[data_len];

	int remain = old_data_len;
	uint32_t *data32 = (uint32_t *)data;
	uint32_t *old32 = (uint32_t *)old_data;

	int i = 0;

	// copy as much data as possible in 32-bit chunks
	while (remain >= 4) {
		data32[i] = old32[i];

		i++;
		remain -= sizeof(uint32_t);
	}

	// copy remaining amount
	while (remain) {
		int offset = old_data_len - remain;
		data[offset] = old_data[offset];

		remain--;
	}

	delete[] old_data;
}

template <>
uint32_t string<utf32>::at(int index) const noexcept {
	uint32_t *data32 = (uint32_t *)data;

	if (index < 0) {
		index += character_ct;
	}
	if ((index < character_ct) && (index >= 0)) {
		return data32[index];
	} else {
		return 0;
	}
}

template <>
const char *string<utf32>::cstring() const noexcept {
	return 0;
}

template <>
const uint16_t *string<utf32>::nstring() const noexcept {
	return 0;
}

template <>
const uint32_t *string<utf32>::wstring() const noexcept {
	return (uint32_t *)data;
}

template <>
void string<utf32>::initChar(uint32_t chr, int index) noexcept {
	uint32_t *data32 = (uint32_t *)data;
	data32[index] = chr;
}

template <>
int string<utf32>::charSize() const noexcept {
	return 4;
}

template <>
string<utf32> string<utf32>::substr(int index, int count) const noexcept {
	string<utf32> result;
	uint32_t *data32 = (uint32_t *)data;
	uint32_t *result32;

	if (index < 0) {
		index += character_ct;
	}

	if (count < 0) {
		if (index >= character_ct) {
			index = character_ct - 1;
		}
		if (index < 0) {
			return result;
		}

		count = -count;
		if (count > (character_ct - index)) {
			count = character_ct - index;
		}
		result.increase(count);

		result32 = (uint32_t *)result.data;

		int beg = index - count + 1;
		for (int i = beg; i <= index; i++) {
			result32[index - i] = data32[i];
		}
		result32[count] = 0;
		result.character_ct = count;
	} else if (count) {
		if (index >= character_ct) {
			return result;
		}
		if (index < 0) {
			index = 0;
		}

		if (count > (character_ct - index)) {
			count = character_ct - index;
		}
		result.increase(count);

		result32 = (uint32_t *)result.data;

		int end = index + count;
		for (int i = index; i < end; i++) {
			result32[i - index] = data32[i];
		}

		result32[count] = 0;
		result.character_ct = count;
	}

	return result;
}

template <>
long string<utf32>::integer(int base, uint32_t decimal) const noexcept {
	if ((base < 2) || (base > 36)) {
		return 0;
	}

	uint32_t *data32 = (uint32_t *)data;

	bool negative = (data32[0] == '-');
	long result = 0;

	int start = (negative || (data32[0] == '+'));

	for (int i = start; i < character_ct; i++) {
		uint32_t chr = data32[i];

		if (isNumeric(chr)) {
			result *= base;
			result += numeralValue(chr);
		} else if (chr == decimal) {
			break;
		} else {
			return 0;
		}
	}

	return (negative ? -result : result);
}

template <>
double string<utf32>::floating(int base, uint32_t decimal) const noexcept {
	if ((base < 2) || (base > 36)) {
		return 0;
	}

	if (!character_ct) {
		return 0;
	}

	bool pastDecimal, pastExponent, negexponent;
	pastDecimal = pastExponent = negexponent = false;

	uint32_t *data32 = (uint32_t *)data;

	bool negative = (data32[0] == '-');
	int start = (negative || (data32[0] == '+'));

	if (start >= character_ct) {
		return 0;
	}

	double result = 0;
	double frac = 1;
	int exponent = 0;

	for (int i = start; i < character_ct; i++) {
		if (!isNumeric(data32[i], base)) {
			if (data32[i] == decimal) {
				if (pastDecimal || pastExponent) {
					return 0;
				} else {
					if (i >= character_ct - 1) {
						return false;
					}
					pastDecimal = true;
				}
			} else if (core::toLower(data32[i]) == 'e') {
				if (pastExponent) {
					return 0;
				} else {
					pastExponent = true;
					negexponent = (data32[i + 1] == '-');
					if (negexponent || (data32[i + 1] == '+')) {
						i++;
					}
				}
			} else {
				return 0;
			}
		} else {
			if (pastExponent) {
				exponent *= base;
				exponent += numeralValue(data32[i]);
			} else if (pastDecimal) {
				frac /= base;
				result += (double)numeralValue(data32[i]) * frac;
			} else {
				result *= base;
				result += numeralValue(data32[i]);
			}
		}
	}

	if (pastExponent) {
		for (int i = 0; i < exponent; i++) {
			if (negexponent) {
				result /= base;
			} else {
				result *= base;
			}
		}
	}

	return (negative ? -result : result);
}

template <>
std::complex<double> string<utf32>::complex(int base, uint32_t decimal) const noexcept {
	if ((base < 2) || (base > 36)) {
		return 0;
	}
	if (!character_ct) {
		return 0;
	}

	uint32_t *data32 = (uint32_t *)data;

	int start = ((data32[0] == '-') || (data32[0] == '+'));
	bool imag = false;
	bool imagEnd = core::toLower(data32[character_ct - 1]) == 'i';
	for (int i = start; i < character_ct; ++i) {
		if (core::toLower(data32[i]) == 'i') {
			imag = true;
		}
		if (((data32[i] == '-') || (data32[i] == '+')) && (base < 14) && i && (core::toLower(data32[i - 1]) != 'e')) {
			if (!(imag ^ imagEnd)) {
				return 0;
			}
			auto sub1 = substr(0, i - imag);
			auto sub2 = substr(i, character_ct - i - imagEnd);

			if (imag) {
				return std::complex<double>(sub2.floating(base, decimal), sub1.floating(base, decimal));
			} else {
				return std::complex<double>(sub1.floating(base, decimal), sub2.floating(base, decimal));
			}
		}
	}

	if (imagEnd) {
		return std::complex<double>(0, substr(0, character_ct - 1).floating(base, decimal));
	} else {
		return floating(base, decimal);
	}
}

/// operators
template <>
string<utf32> &string<utf32>::operator+=(const string<utf32> &other) noexcept {
	int new_size = character_ct + other.character_ct + 1;
	this->increase(new_size << 2);

	uint32_t *data32 = (uint32_t *)data;
	uint32_t *other32 = (uint32_t *)other.data;

	for (int i = 0; i < other.character_ct; i++) {
		data32[character_ct + i] = other32[i];
	}
	data32[new_size - 1] = 0;

	character_ct += other.character_ct;

	return *this;
}

/// analyzers
template <>
int string<utf32>::count(const string<utf32> &other) const noexcept {
	if (!other.character_ct) {
		return 0;
	}

	uint32_t *data32 = (uint32_t *)data;
	uint32_t *other32 = (uint32_t *)other.data;

	int occurrence = 0;

	int other_i = 0;
	for (int i = 0; i < character_ct; i++) {
		if (data32[i] == other32[other_i]) {
			other_i++;
			if (other_i >= other.character_ct) {
				occurrence++;
			}

			if (!occurrence) {
				return (i - other_i + 1);
			}
		} else {
			if ((character_ct - i) <= other.character_ct) {
				return occurrence;
			}

			other_i = 0;
		}
	}

	return occurrence;
}

template <>
int string<utf32>::findBefore(const string<utf32> &other, int index, int occurrence) const noexcept {
	if (index < 0) {
		index += character_ct;
	}
	if (!other.character_ct || (occurrence < 1) || (index < 0)) {
		return -1;
	}

	uint32_t *data32 = (uint32_t *)data;
	uint32_t *other32 = (uint32_t *)other.data;

	if (index > (character_ct - other.character_ct)) {
		index = character_ct - other.character_ct;
	}

	int other_i = other.character_ct - 1;
	for (int i = index; i < character_ct; i--) {
		if (data32[i] == other32[other_i]) {

			if (!other_i) {
				occurrence--;

				if (occurrence && (i < other.character_ct)) {
					i = 0;
				}
				other_i = other.character_ct - 1;
			} else {
				other_i--;
			}

			if (!occurrence) {
				return i;
			}
		} else {
			if (occurrence && (i < other.character_ct)) {
				return -1;
			}
			other_i = other.character_ct - 1;
		}
	}

	return -1;
}

template <>
int string<utf32>::type(int base, uint32_t decimal) const noexcept {
	if ((base < 2) || (base > 36) || !character_ct) {
		return zstr::string;
	}

	bool pastDecimal, pastExponent, imag, ir;
	pastDecimal = pastExponent = imag = ir = false;

	uint32_t *data32 = (uint32_t *)data;

	int start = ((data32[0] == '-') || (data32[0] == '+'));

	if (start >= character_ct) {
		return zstr::string;
	}

	for (int i = start; i < character_ct; i++) {
		if (!isNumeric(data32[i], 10)) {
			if (data32[i] == decimal) {
				if (pastDecimal || pastExponent) {
					return zstr::string;
				} else {
					if ((i >= character_ct - 1) || (core::toLower(data32[i + 1]) == 'i')) {
						return zstr::string;
					}
					pastDecimal = true;
				}
			} else if (core::toLower(data32[i]) == 'e') {
				if (pastExponent) {
					return zstr::string;
				} else {
					pastExponent = true;
					if ((data32[i + 1] == '+') || (data32[i + 1] == '-')) {
						i++;
					}
				}
			} else if (core::toLower(data32[i]) == 'i') {
				if (imag) {
					return zstr::string;
				} else {
					pastExponent = pastDecimal = false;
					imag = true;
				}
			} else if ((data32[i] == '-') || (data32[i] == '+')) {
				if (ir || (i >= character_ct - 1)) {
					return zstr::string;
				} else {
					pastDecimal = pastExponent = false;
					ir = true;
				}
			} else {
				return zstr::string;
			}
		}
	}

	if (imag) {
		return zstr::complex;
	} else {
		return pastDecimal ? zstr::floating : zstr::integer;
	}
}

/// mutators
template <>
string<utf32> &string<utf32>::operator=(const string<utf32> &other) noexcept {
	if (&other != this) {
		int new_len = (other.character_ct + 1) << 2;
		this->increase(new_len);
		// data = new uint8_t[data_len];

		character_ct = other.character_ct;

		uint32_t *data32 = (uint32_t *)data;
		uint32_t *other32 = (uint32_t *)other.data;

		for (int i = 0; i < character_ct; i++) {
			data32[i] = other32[i];
		}
	}

	return *this;
}

template <>
string<utf32> &string<utf32>::append(uint32_t chr) noexcept {
	if (chr) {
		increase(character_ct + 1);
		uint32_t *data32 = (uint32_t *)data;
		data32[character_ct] = chr;
		++character_ct;
		data32[character_ct] = 0;
	}
	return *this;
}

template <>
string<utf32> &string<utf32>::insert(const string<utf32> &other, int index) noexcept // insert before index
{
	if (index < 0) {
		index += character_ct;
	}
	if (!other.character_ct || index < 0) {
		return *this;
	}

	if (index >= character_ct) {
		index = character_ct;
	}

	int start = index + other.character_ct;
	int end = character_ct + other.character_ct;
	increase(end);

	uint32_t *data32 = (uint32_t *)data;
	uint32_t *other32 = (uint32_t *)other.data;

	for (int i = end - 1; i >= start; i--) {
		data32[i] = data32[i - other.character_ct];
	}

	end = index + other.character_ct;
	for (int i = index; i < end; i++) {
		data32[i] = other32[i - index];
	}

	character_ct += other.character_ct;
	data32[character_ct] = 0;

	return *this;
}

template <>
string<utf32> &string<utf32>::remove(int index, int count) noexcept {
	if (count) {
		if (index < 0) {
			index += character_ct;
		}
		if (index >= character_ct) {
			return *this;
		}

		int start, end, offset;

		if (count < 0) {
			if ((index + count) > character_ct) {
				start = 0;
				offset = index;
			} else {
				start = index + count;
				offset = -count;
			}
		} else {
			if ((index + count) > character_ct) {
				offset = character_ct - index;
			} else {
				offset = count;
			}

			start = index;
		}

		uint32_t *data32 = (uint32_t *)data;

		end = character_ct - offset;
		for (int i = start; i < end; i++) {
			data32[i] = data32[i + offset];
		}

		data32[end] = 0;
		character_ct = end;
	}

	return *this;
}

template <>
string<utf32> &string<utf32>::replace(int index, int count, const string<utf32> &other) noexcept {
	if (!count) {
		return *this;
	}
	if (count < 0) {
		index += count;
		count = -count;
	}
	if (index < 0) {
		index += character_ct;
		if (index < 0) {
			index = 0;
		}
	}

	if (index + count > character_ct) {
		count = character_ct - index;
	}

	int offset = other.character_ct - count;
	if (offset > 0) {
		// Make space for the new data
		increase(character_ct + other.character_ct - count);
	}

	uint32_t *data32 = (uint32_t *)data;
	uint32_t *other32 = (uint32_t *)other.data;

	if (offset > 0) {
		// Make space for the new data
		for (int i = character_ct - 1; i >= index + count; --i) {
			data32[i + offset] = data32[i];
		}
	} else if (offset < 0) {
		// Shift the old data in
		for (int i = index + count; i < character_ct; ++i) {
			data32[i + offset] = data32[i];
		}
	}

	// Insert the new data
	for (int i = 0; i < other.character_ct; ++i) {
		data32[i + index] = other32[i];
	}

	character_ct += offset;
	data32[character_ct] = '\0';

	return *this;
}

template <>
string<utf32> &string<utf32>::toUpper() noexcept {
	uint32_t *data32 = (uint32_t *)data;

	for (int i = 0; i < character_ct; i++) {
		data32[i] = core::toUpper(data32[i]);
	}

	return *this;
}

template <>
string<utf32> &string<utf32>::toLower() noexcept {
	uint32_t *data32 = (uint32_t *)data;

	for (int i = 0; i < character_ct; i++) {
		data32[i] = core::toLower(data32[i]);
	}

	return *this;
}

template <>
string<utf32> &string<utf32>::toCamel() noexcept {
	uint32_t *data32 = (uint32_t *)data;

	bool doLower = false;
	for (int i = 0; i < character_ct; i++) {
		uint32_t ch = data32[i];
		data32[i] = (doLower ? core::toLower(ch) : core::toUpper(ch, true));

		doLower = isAlphaNumeric(ch) || (ch == '_');
	}

	return *this;
}

template <>
string<utf32> &string<utf32>::read(std::istream &stream, uint32_t delim) noexcept {
	character_ct = 0;
	uint32_t *data32 = (uint32_t *)data;
	data32[0] = 0;

	if (stream.fail() || stream.eof()) {
		return *this;
	}

	uint32_t last = stream.get();

	while (!stream.eof() && last && (delim ? (last == delim) : isWhiteSpace(last))) {
		last = stream.get();
	}

	while (!stream.eof() && last && !(delim ? (last == delim) : isWhiteSpace(last))) {
		uint8_t c[4];
		c[0] = last;

		int len = lenFromUTF8(c);
		if (len) {
			for (int i = 1; i < len; i++) {
				c[i] = stream.get();
			}

			last = fromUTF8(c);
		}

		increase(character_ct);
		data32 = (uint32_t *)data;
		data32[character_ct++] = last;

		last = stream.get();
	}

	increase(character_ct);
	data32 = (uint32_t *)data;
	data32[character_ct] = 0;

	return *this;
}

template <>
string<utf32> &string<utf32>::readln(std::istream &stream) noexcept {
	character_ct = 0;

	uint32_t *data32 = (uint32_t *)data;
	data32[0] = 0;

	if (stream.fail() || stream.eof()) {
		return *this;
	}

	uint32_t last = stream.get();

	while (!stream.eof()) {
		if ((last == '\n') || (last == '\r')) {
			break;
		}

		uint8_t c[4];
		c[0] = last;

		int len = lenFromUTF8(c);
		if (len) {
			for (int i = 1; i < len; i++) {
				c[i] = stream.get();
			}

			last = fromUTF8(c);
		}

		increase(character_ct);
		data32 = (uint32_t *)data;
		data32[character_ct++] = last;

		last = stream.get();
	}

	increase(character_ct);
	data32 = (uint32_t *)data;
	data32[character_ct] = 0;

	return *this;
}

template <>
string<utf32> &string<utf32>::readall(std::istream &stream) noexcept {
	character_ct = 0;

	uint32_t *data32 = (uint32_t *)data;
	data32[0] = 0;

	if (stream.fail() || stream.eof()) {
		return *this;
	}

	uint32_t last = stream.get();

	while (!stream.eof()) {
		uint8_t c[4];
		c[0] = last;

		int len = lenFromUTF8(c);
		if (len) {
			for (int i = 1; i < len; i++) {
				c[i] = stream.get();
			}

			last = fromUTF8(c);
		}

		increase(character_ct);
		data32 = (uint32_t *)data;
		data32[character_ct++] = last;

		last = stream.get();
	}

	increase(character_ct);
	data32 = (uint32_t *)data;
	data32[character_ct] = 0;

	return *this;
}

template <>
void string<utf32>::initInt(long long value, int base, int padSize) noexcept {
	uint8_t ibuf[Z_STR_INT_BUFSIZE];
	if ((base < 2) || (base > 36)) {
		base = 10;
	}

	bool negative = false;
	if (value < 0) {
		value = -value;
		negative = true;
	}

	int ibufsiz = integralBuf(value, base, ibuf);

	// initialize string data
	character_ct = ibufsiz + negative;
	if (character_ct < padSize) {
		character_ct += (padSize -= character_ct);
	} else {
		padSize = 0;
	}

	data_len = (character_ct + 1) * this->charSize();
	data = new uint8_t[data_len];

	if (negative) {
		this->initChar('-', 0);
	}

	int pos = negative;

	for (int i = 0; i < padSize; i++) {
		this->initChar('0', pos++);
	}

	for (int i = 0; i < ibufsiz; i++) {
		this->initChar(ibuf[ibufsiz - i - 1], pos++);
	}
}

template <>
void string<utf32>::initPointer(void *pointer) noexcept {
	uint8_t pbuf[Z_STR_INT_BUFSIZE];

	union ptv {
		void *pval;
		unsigned long ival;
	};
	ptv ptr;
	ptr.pval = pointer;

	int pbufsiz = integralBuf(ptr.ival, 16, pbuf);
	int padSize;

	// initialize string data
	if (Z_STR_POINTER_FORCE && (pbufsiz < Z_STR_POINTER_CHARS)) {
		padSize = Z_STR_POINTER_CHARS - pbufsiz;
	} else {
		padSize = 0;
	}

	character_ct = padSize + pbufsiz + 2;
	data_len = (character_ct + 1) * this->charSize();
	data = new uint8_t[data_len];

	// if (negative) this->initChar('-', 0);

	int pos = 0;

	this->initChar('0', pos++);
	this->initChar('x', pos++);

	for (int i = 0; i < padSize; i++) {
		this->initChar('0', pos++);
	}

	for (int i = 0; i < pbufsiz; i++) {
		this->initChar(pbuf[pbufsiz - i - 1], pos++);
	}

	initChar(0, character_ct);
}

template <>
void string<utf32>::initFloat(double value, int base, int precision, bool scientific, int padSize) noexcept {
	uint8_t ibuf[Z_STR_INT_BUFSIZE];
	uint8_t fbuf[Z_STR_FLOAT_BUFSIZE];
	uint8_t ebuf[Z_STR_EXP_BUFSIZE];

	data = 0;

	bool force = true;

	if ((base < 2) || (base > 36)) {
		base = 10;
	}
	if (precision <= 0) {
		precision = Z_STR_FLOAT_PRECISION;
		force = false;
	}

	bool negative = (value < 0.0);
	if (negative) {
		value = -value;
	}

	int exponent = 0;
	bool negexponent = false;

	// handle very small or very large numbers
	if (value && scientific) {
		double minVal = 1.0;
		double maxVal = 1.0;
		for (int i = 0; i < precision; i++) {
			minVal /= base;
			maxVal *= base << 1;
		}
		minVal *= (double)base / 2.0;

		if (value < minVal) {
			negexponent = true;
			do {
				value *= base;
				++exponent;
			} while (value < 1.0);
		} else if (value > maxVal) {
			do {
				value /= base;
				++exponent;
			} while (value >= base);
		}
	}

	// split up parts of the number
	double intTemp = 0.0;
	double fractional = modf(value, &intTemp);
	unsigned long integral = intTemp;

	bool overflow = false;
	int ibufsiz = integralBuf(integral, base, ibuf);
	int fbufsiz = fractionalBuf(fractional, base, precision, force, fbuf, &overflow);
	if (overflow) {
		trimFloatBuf(base, force, fbuf, &fbufsiz, ibuf, &ibufsiz);
	}
	int ebufsiz = exponent ? integralBuf(exponent, base, ebuf) : 0;
	// initialize string data
	character_ct = ibufsiz + negative + (bool)fractional + fbufsiz + (bool)exponent + negexponent + ebufsiz;
	if (character_ct < padSize) {
		character_ct += (padSize -= character_ct);
	} else {
		padSize = 0;
	}

	data_len = (character_ct + 1) * this->charSize();
	data = new uint8_t[data_len];
	if (negative) {
		this->initChar('-', 0);
	}

	int pos = negative;

	for (int i = 0; i < padSize; i++) {
		this->initChar('0', pos++);
	}

	for (int i = 0; i < ibufsiz; i++) {
		this->initChar(ibuf[ibufsiz - i - 1], pos++);
	}

	if (fbufsiz) {
		this->initChar('.', pos++);

		for (int i = 0; i < fbufsiz; i++) {
			this->initChar(fbuf[i], pos++);
		}
	}

	if (exponent) {
		this->initChar('e', pos++);
		if (negexponent) {
			this->initChar('-', pos++);
		}

		for (int i = 0; i < ebufsiz; i++) {
			this->initChar(ebuf[ebufsiz - i - 1], pos++);
		}
	}

	this->initChar(0, pos);
}

template <>
void string<utf32>::initComplex(const std::complex<double> &value, int base, int precision, bool scientific, int padSize) noexcept {
	data_len = 4;
	data = new uint8_t[data_len];
	*((uint32_t *)data) = 0;
	character_ct = 0;

	if (value.real() && value.imag()) {
		operator=(string<utf32>(value.real(), base, precision, scientific, padSize));
		if (value.imag() > 0) {
			operator+=("+");
		}
		operator+=(string<utf32>(value.imag(), base, precision, scientific, padSize));
		operator+=("i");
	} else if (value.imag()) {
		operator=(string<utf32>(value.imag(), base, precision, scientific, padSize));
		operator+=("i");
	} else {
		operator=(string<utf32>(value.real(), base, precision, scientific, padSize));
	}
}

template <>
size_t string<utf32>::size() const noexcept {
	return sizeof(*this) + (data_len * sizeof(uint8_t));
}

template <>
int string<utf32>::length() const noexcept {
	return character_ct;
}

template <>
int string<utf32>::chars() const noexcept {
	return character_ct;
}

template <>
bool string<utf32>::foundAt(const string<utf32> &other, int index) const noexcept {
	if (index < 0) {
		index += character_ct;
	}
	if ((character_ct - index) < other.character_ct || index < 0) {
		return false;
	}

	uint32_t *data32 = (uint32_t *)data;
	uint32_t *other32 = (uint32_t *)other.data;

	const int charSz = this->charSize();

	int end = (other.character_ct * charSz) >> 2;
	for (int i = 0; i < end; i++) {
		if (data32[i + index] != other32[i]) {
			return false;
		}
	}

	return true;
}

template <>
bool string<utf32>::foundEndAt(const string<utf32> &other, int index) const noexcept {
	return foundAt(other, index - other.character_ct + 1);
}

/// mutators
template <>
string<utf32> &string<utf32>::remove(const string &other, int occurrence) noexcept {
	if (occurrence > 0) // remove one occurrence
	{
		int pos = this->find(other, occurrence);

		if (pos >= 0) {
			this->remove(pos, other.length());
		}
	} else if (!occurrence) // remove all occurrences
	{
		int pos = this->find(other, 1);

		while (pos >= 0) {
			this->remove(pos, other.length());
			pos = this->findAfter(other, pos, 1);
		}
	}

	return *this;
}

template <>
string<utf32> &string<utf32>::truncate(int index) noexcept {
	if (index >= character_ct) {
		return *this;
	}
	if (index < -character_ct) {
		index = 0;
	} else if (index < 0) {
		index += character_ct;
	}

	uint32_t *data32 = (uint32_t *)data;
	data32[index] = 0;
	character_ct = index;
	return *this;
}

template <>
string<utf32> &string<utf32>::replace(const string<utf32> &findStr, const string<utf32> &replStr, int occurrence) noexcept {
	if (occurrence > 0) // replace one occurrence
	{
		int pos = this->find(findStr, occurrence);

		if (pos >= 0) {
			this->replace(pos, findStr.length(), replStr);
		}
	} else if (!occurrence) // replace all occurrences
	{
		int pos = this->find(findStr, 1);

		while (pos >= 0) {
			this->replace(pos, findStr.length(), replStr);
			pos = this->findAfter(findStr, pos + replStr.length(), 1);
		}
	}

	return *this;
}

template <>
string<utf32> &string<utf32>::padLeftIn(const string<utf32> &other, int padSize) noexcept {
	if (padSize <= character_ct) {
		return *this;
	}

	string<utf32> padStr;

	int padChars = padSize - character_ct;

	while (padChars >= other.character_ct) {
		padStr += other;
		padChars -= other.character_ct;
	}

	if (padChars > 0) {
		padStr += other.substr(0, padChars);
	}

	return this->insert(padStr, 0);
}

template <>
string<utf32> &string<utf32>::padRightIn(const string<utf32> &other, int padSize) noexcept {
	if (padSize <= character_ct) {
		return *this;
	}

	string<utf32> padStr;

	int padChars = padSize - character_ct;

	while (padChars >= other.character_ct) {
		padStr += other;
		padChars -= other.character_ct;
	}

	if (padChars > 0) {
		padStr += other.substr(0, padChars);
	}

	return operator+=(padStr);
}

template <>
void string<utf32>::clear() noexcept {
	uint32_t *data32 = (uint32_t *)data;
	data32[0] = 0;
	character_ct = 0;
}

template <>
string<utf32> &string<utf32>::cutDuplicates(const string<utf32> &other) noexcept {
	int pos = this->find(other, 1);

	while (pos >= 0) {
		int opos = pos + other.length();
		while (this->foundAt(other, opos)) {
			this->remove(opos, other.length());
		}
		pos = this->findAfter(other, opos, 1);
	}

	return *this;
}

/// operators
template <>
string<utf32> string<utf32>::operator+(const string<utf32> &other) const noexcept {
	string<utf32> result(*this);
	result += other;

	return result;
}

template <>
bool string<utf32>::operator==(const string<utf32> &other) const noexcept {
	if (character_ct != other.character_ct) {
		return false;
	}
	if (this == &other) {
		return true;
	}

	auto data32 = (uint32_t *)data;
	auto other32 = (uint32_t *)other.data;

	for (int i = 0; i < character_ct; ++i) {
		if (data32[i] != other32[i]) {
			return false;
		}
	}
	return true;
}

template <>
bool string<utf32>::operator>(const string<utf32> &other) const noexcept {
	int len = (character_ct > other.character_ct) ? other.character_ct : character_ct;
	if (this == &other) {
		return false;
	}

	auto data32 = (uint32_t *)data;
	auto other32 = (uint32_t *)other.data;

	for (int i = 0; i < len; ++i) {
		if (data32[i] != other32[i]) {
			return data32[i] > other32[i];
		}
	}
	return character_ct > other.character_ct;
}

template <>
bool string<utf32>::operator<(const string<utf32> &other) const noexcept {
	int len = (character_ct > other.character_ct) ? other.character_ct : character_ct;
	if (this == &other) {
		return false;
	}

	auto data32 = (uint32_t *)data;
	auto other32 = (uint32_t *)other.data;

	for (int i = 0; i < len; ++i) {
		if (data32[i] != other32[i]) {
			return data32[i] < other32[i];
		}
	}
	return character_ct < other.character_ct;
}

template <>
hash32 string<utf32>::hash() const noexcept {
	return string<utf8>(*this).hash();
}

} // namespace core
} // namespace z
