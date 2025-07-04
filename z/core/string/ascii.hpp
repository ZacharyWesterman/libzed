#pragma once

namespace z {
namespace core {
template <>
void string<ascii>::increase(int max_chars) noexcept {
	int goal = max_chars + 1; // account for null byte at the end
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

	memcpy(data, old_data, old_data_len);

	delete[] old_data;
}

template <>
const char *string<ascii>::cstring() const noexcept {
	return (char *)data;
}

template <>
const uint16_t *string<ascii>::nstring() const noexcept {
	return nullptr;
}

template <>
const uint32_t *string<ascii>::wstring() const noexcept {
	return nullptr;
}

template <>
void string<ascii>::initChar(uint32_t chr, int index) noexcept {
	data[index] = chr;
}

template <>
int string<ascii>::charSize() const noexcept {
	return 1;
}

template <>
uint32_t string<ascii>::at(int index) const noexcept {
	if (index < 0) {
		index += character_ct;
	}
	if ((index < character_ct) && (index >= 0)) {
		return data[index];
	} else {
		return 0;
	}
}

/// operators

template <>
string<ascii> &string<ascii>::operator+=(const string<ascii> &other) noexcept {
	increase(character_ct + other.character_ct);
	memcpy((data + character_ct), other.data, other.character_ct);
	character_ct += other.character_ct;
	return *this;
}

/// analyzers
template <>
int string<ascii>::count(const string<ascii> &other) const noexcept {
	if (!other.character_ct) {
		return 0;
	}

	int occurrence = 0;

	int other_i = 0;
	for (int i = 0; i < character_ct; i++) {
		if (data[i] == other.data[other_i]) {
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
int string<ascii>::findBefore(const string<ascii> &other, int index, int occurrence) const noexcept {
	if (index < 0) {
		index += character_ct;
	}
	if (!other.character_ct || (occurrence < 1) || (index < 0)) {
		return -1;
	}

	if (index > (character_ct - other.character_ct)) {
		index = character_ct - other.character_ct;
	}

	int other_i = other.character_ct - 1;
	for (int i = index; i < character_ct; i--) {
		if (data[i] == other.data[other_i]) {

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

/// mutators
template <>
string<ascii> &string<ascii>::operator=(const string<ascii> &other) noexcept {
	if (&other != this) {
		increase(other.character_ct);
		character_ct = other.character_ct;
		memcpy(data, other.data, other.character_ct + 1);
	}
	return *this;
}

template <>
string<ascii> &string<ascii>::append(uint32_t chr) noexcept {
	if (chr) {
		increase(character_ct + 1);
		data[character_ct] = (chr > 0xFF) ? '?' : chr;
		++character_ct;
		data[character_ct] = 0;
	}
	return *this;
}

template <>
string<ascii> &string<ascii>::insert(const string<ascii> &other, int index) noexcept // insert before index
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

	for (int i = end - 1; i >= start; i--) {
		data[i] = data[i - other.character_ct];
	}

	end = index + other.character_ct;
	for (int i = index; i < end; i++) {
		data[i] = other.data[i - index];
	}

	character_ct += other.character_ct;
	data[character_ct] = 0;

	return *this;
}

template <>
string<ascii> &string<ascii>::remove(int index, int count) noexcept {
	if (count) {
		if (index < 0) {
			index += character_ct;
		}
		if (index >= character_ct || index < 0) {
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

		end = character_ct - offset;
		for (int i = start; i < end; i++) {
			data[i] = data[i + offset];
		}

		data[end] = 0;
		character_ct = end;
	}

	return *this;
}

template <>
string<ascii> &string<ascii>::replace(int index, int count, const string<ascii> &other) noexcept {
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

		for (int i = character_ct - 1; i >= index + count; --i) {
			data[i + offset] = data[i];
		}
	} else if (offset < 0) {
		// Shift the old data in
		for (int i = index + count; i < character_ct; ++i) {
			data[i + offset] = data[i];
		}
	}

	// Insert the new data
	for (int i = 0; i < other.character_ct; ++i) {
		data[i + index] = other.data[i];
	}

	character_ct += offset;
	data[character_ct] = '\0';
	return *this;
}

template <>
void string<ascii>::initInt(long long value, int base, int padSize) noexcept {
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

	initChar(0, character_ct);
}

template <>
void string<ascii>::initPointer(void *pointer) noexcept {
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
}

template <>
void string<ascii>::initFloat(double value, int base, int precision, bool scientific, int padSize) noexcept {
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
	// size_t pos = 0;
	int pos = negative;
	//
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
void string<ascii>::initComplex(const std::complex<double> &value, int base, int precision, bool scientific, int padSize) noexcept {
	data_len = 4;
	data = new uint8_t[data_len];
	*((uint32_t *)data) = 0;
	character_ct = 0;

	if (value.real() && value.imag()) {
		operator=(string<ascii>(value.real(), base, precision, scientific, padSize));
		if (value.imag() > 0) {
			operator+=("+");
		}
		operator+=(string<ascii>(value.imag(), base, precision, scientific, padSize));
		operator+=("i");
	} else if (value.imag()) {
		operator=(string<ascii>(value.imag(), base, precision, scientific, padSize));
		operator+=("i");
	} else {
		operator=(string<ascii>(value.real(), base, precision, scientific, padSize));
	}
}

template <>
size_t string<ascii>::size() const noexcept {
	return sizeof(*this) + (data_len * sizeof(uint8_t));
}

template <>
int string<ascii>::length() const noexcept {
	return character_ct;
}

template <>
int string<ascii>::chars() const noexcept {
	return character_ct;
}

template <>
string<ascii> string<ascii>::substr(int index, int count) const noexcept {
	string<ascii> result;

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

		int beg = index - count + 1;
		for (int i = beg; i <= index; i++) {
			result.data[index - i] = data[i];
		}
		result.data[count] = 0;
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

		int end = index + count;
		for (int i = index; i < end; i++) {
			result.data[i - index] = data[i];
		}

		result.data[count] = 0;
		result.character_ct = count;
	}

	return result;
}

template <>
long string<ascii>::integer(int base, uint32_t decimal) const noexcept {
	if ((base < 2) || (base > 36)) {
		return 0;
	}

	bool negative = (data[0] == '-');
	long result = 0;

	int start = (negative || (data[0] == '+'));

	for (int i = start; i < character_ct; i++) {
		uint32_t chr = data[i];

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
double string<ascii>::floating(int base, uint32_t decimal) const noexcept {
	if ((base < 2) || (base > 36)) {
		return 0;
	}

	if (!character_ct) {
		return 0;
	}

	bool pastDecimal, pastExponent, negexponent;
	pastDecimal = pastExponent = negexponent = false;

	bool negative = (data[0] == '-');
	int start = (negative || (data[0] == '+'));

	if (start >= character_ct) {
		return 0;
	}

	double result = 0;
	double frac = 1;
	int exponent = 0;

	for (int i = start; i < character_ct; i++) {
		if (!isNumeric(data[i], base)) {
			if (data[i] == decimal) {
				if (pastDecimal || pastExponent) {
					return 0;
				} else {
					if (i >= character_ct - 1) {
						return false;
					}
					pastDecimal = true;
				}
			} else if (core::toLower(data[i]) == 'e') {
				if (pastExponent) {
					return 0;
				} else {
					pastExponent = true;
					negexponent = (data[i + 1] == '-');
					if (negexponent || (data[i + 1] == '+')) {
						i++;
					}
				}
			} else {
				return 0;
			}
		} else {
			if (pastExponent) {
				exponent *= base;
				exponent += numeralValue(data[i]);
			} else if (pastDecimal) {
				frac /= base;
				result += (double)numeralValue(data[i]) * frac;
			} else {
				result *= base;
				result += numeralValue(data[i]);
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
std::complex<double> string<ascii>::complex(int base, uint32_t decimal) const noexcept {
	if ((base < 2) || (base > 36)) {
		return 0;
	}
	if (!character_ct) {
		return 0;
	}

	int start = ((data[0] == '-') || (data[0] == '+'));
	bool imag = false;
	bool imagEnd = core::toLower(data[character_ct - 1]) == 'i';
	for (int i = start; i < character_ct; ++i) {
		if (core::toLower(data[i]) == 'i') {
			imag = true;
		}
		if (((data[i] == '-') || (data[i] == '+')) && (base < 14) && i && (core::toLower(data[i - 1]) != 'e')) {
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

/// analyzers
template <>
bool string<ascii>::foundAt(const string<ascii> &other, int index) const noexcept {
	if (index < 0) {
		index += character_ct;
	}
	if ((character_ct - index) < other.character_ct || index < 0) {
		return false;
	}

	for (int i = 0; i < other.character_ct; i++) {
		if (data[i + index] != other.data[i]) {
			return false;
		}
	}

	return true;
}

template <>
bool string<ascii>::foundEndAt(const string<ascii> &other, int index) const noexcept {
	return foundAt(other, index - other.character_ct + 1);
}

template <>
int string<ascii>::type(int base, uint32_t decimal) const noexcept {
	if ((base < 2) || (base > 36) || !character_ct) {
		return zstr::string;
	}

	bool pastDecimal, pastExponent, imag, ir;
	pastDecimal = pastExponent = imag = ir = false;

	int start = ((data[0] == '-') || (data[0] == '+'));

	if (start >= character_ct) {
		return zstr::string;
	}

	for (int i = start; i < character_ct; i++) {
		if (!isNumeric(data[i], 10)) {
			if (data[i] == decimal) {
				if (pastDecimal || pastExponent) {
					return zstr::string;
				} else {
					if ((i >= character_ct - 1) || (core::toLower(data[i + 1]) == 'i')) {
						return zstr::string;
					}
					pastDecimal = true;
				}
			} else if (core::toLower(data[i]) == 'e') {
				if (pastExponent) {
					return zstr::string;
				} else {
					pastExponent = true;
					if ((data[i + 1] == '+') || (data[i + 1] == '-')) {
						i++;
					}
				}
			} else if (core::toLower(data[i]) == 'i') {
				if (imag) {
					return zstr::string;
				} else {
					pastExponent = pastDecimal = false;
					imag = true;
				}
			} else if ((data[i] == '-') || (data[i] == '+')) {
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
string<ascii> &string<ascii>::remove(const string &other, int occurrence) noexcept {
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
string<ascii> &string<ascii>::truncate(int index) noexcept {
	if (index >= character_ct) {
		return *this;
	}
	if (index < -character_ct) {
		index = 0;
	} else if (index < 0) {
		index += character_ct;
	}

	data[index] = 0;
	character_ct = index;
	return *this;
}

template <>
string<ascii> &string<ascii>::replace(const string<ascii> &findStr, const string<ascii> &replStr, int occurrence) noexcept {
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
string<ascii> &string<ascii>::padLeftIn(const string<ascii> &other, int padSize) noexcept {
	if (padSize <= character_ct) {
		return *this;
	}

	string<ascii> padStr;

	int padChars = padSize - character_ct;

	while (padChars >= other.character_ct) {
		padStr += other;
		padChars -= other.character_ct;
	}

	if (padChars) {
		padStr += other.substr(0, padChars);
	}

	return this->insert(padStr, 0);
}

template <>
string<ascii> &string<ascii>::padRightIn(const string<ascii> &other, int padSize) noexcept {
	if (padSize <= character_ct) {
		return *this;
	}

	string<ascii> padStr;

	int padChars = padSize - character_ct;

	while (padChars >= other.character_ct) {
		padStr += other;
		padChars -= other.character_ct;
	}

	if (padChars) {
		padStr += other.substr(0, padChars);
	}

	return operator+=(padStr);
}

template <>
void string<ascii>::clear() noexcept {
	data[0] = 0;
	character_ct = 0;
}

template <>
string<ascii> &string<ascii>::cutDuplicates(const string<ascii> &other) noexcept {
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

template <>
string<ascii> &string<ascii>::toUpper() noexcept {
	for (int i = 0; i < character_ct; i++) {
		data[i] = core::toUpper(data[i]);
	}

	return *this;
}

template <>
string<ascii> &string<ascii>::toLower() noexcept {
	for (int i = 0; i < character_ct; i++) {
		data[i] = core::toLower(data[i]);
	}

	return *this;
}

template <>
string<ascii> &string<ascii>::toCamel() noexcept {
	bool doLower = false;
	for (int i = 0; i < character_ct; i++) {
		auto ch = data[i];
		data[i] = (doLower ? core::toLower(ch) : core::toUpper(ch));

		doLower = isAlphaNumeric(ch) || (ch == '_');
	}

	return *this;
}

/// operators
template <>
string<ascii> string<ascii>::operator+(const string<ascii> &other) const noexcept {
	string<ascii> result(*this);
	result += other;

	return result;
}

template <>
string<ascii> &string<ascii>::read(std::istream &stream, uint32_t delim) noexcept {
	character_ct = 0;
	data[0] = 0;

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
		data[character_ct++] = (last > 0xFF) ? '?' : last;

		last = stream.get();
	}

	increase(character_ct);
	data[character_ct] = 0;

	return *this;
}

template <>
string<ascii> &string<ascii>::readln(std::istream &stream) noexcept {
	character_ct = 0;
	data[0] = 0;

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
		data[character_ct++] = (last > 0xFF) ? '?' : last;

		last = stream.get();
	}

	increase(character_ct);
	data[character_ct] = 0;

	return *this;
}

template <>
string<ascii> &string<ascii>::readall(std::istream &stream) noexcept {
	character_ct = 0;
	data[0] = 0;

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
		data[character_ct++] = (last > 0xFF) ? '?' : last;

		last = stream.get();
	}

	increase(character_ct);
	data[character_ct] = 0;

	return *this;
}

template <>
bool string<ascii>::operator==(const string<ascii> &other) const noexcept {
	if (character_ct != other.character_ct) {
		return false;
	}
	if (this == &other) {
		return true;
	}

	for (int i = 0; i < character_ct; ++i) {
		if (data[i] != other.data[i]) {
			return false;
		}
	}
	return true;
}

template <>
bool string<ascii>::operator>(const string<ascii> &other) const noexcept {
	int len = (character_ct > other.character_ct) ? other.character_ct : character_ct;
	if (this == &other) {
		return false;
	}

	for (int i = 0; i < len; ++i) {
		if (data[i] != other.data[i]) {
			return data[i] > other.data[i];
		}
	}
	return character_ct > other.character_ct;
}

template <>
bool string<ascii>::operator<(const string<ascii> &other) const noexcept {
	int len = (character_ct > other.character_ct) ? other.character_ct : character_ct;
	if (this == &other) {
		return false;
	}

	for (int i = 0; i < len; ++i) {
		if (data[i] != other.data[i]) {
			return data[i] < other.data[i];
		}
	}
	return character_ct < other.character_ct;
}

template <>
hash32 string<ascii>::hash() const noexcept {
	return crc32(reinterpret_cast<const char *>(data), character_ct);
}

} // namespace core
} // namespace z
