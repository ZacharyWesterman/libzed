#pragma once

template <encoding E>
string<E>::~string()
{
	delete[] data;
}

template <encoding E>
constexpr encoding string<E>::format() const
{
	return E;
}

template <encoding E>
const uint32_t string<E>::operator[](size_t index) const
{
	return this->at(index);
}

template <encoding E>
template <typename INT, typename>
string<E>::string(INT value, int base, int padSize)
{
	uint8_t ibuf[Z_STR_INT_BUFSIZE];
	if ((base < 2) || (base > 36)) base = 10;

	bool negative = false;
	if (value < 0)
	{
		value = -value;
		negative = true;
	}

	size_t ibufsiz = integralBuf(value, base, ibuf);

	//initialize string data
	character_ct = ibufsiz + negative;
	if (character_ct < padSize)
		character_ct += (padSize -= character_ct);
	else
		padSize = 0;

	data_len = (character_ct + 1) * this->charSize();
	data = new uint8_t[data_len];

	if (negative) this->initChar('-', 0);

	size_t pos = negative;

	for (size_t i=0; i<padSize; i++)
		this->initChar('0',pos++);

	for (size_t i=0; i<ibufsiz; i++)
		this->initChar(ibuf[ibufsiz-i-1], pos++);
}

template <encoding E>
template <typename PTR, typename>
string<E>::string(PTR pointer)
{
	uint8_t pbuf[Z_STR_INT_BUFSIZE];

	union ptv
	{
		PTR pval;
		unsigned long ival;
	};
	ptv ptr;
	ptr.pval = pointer;

	size_t pbufsiz = integralBuf(ptr.ival, 16, pbuf);
	size_t padSize;

	//initialize string data
	if (Z_STR_POINTER_FORCE && (pbufsiz < Z_STR_POINTER_CHARS))
		padSize = Z_STR_POINTER_CHARS - pbufsiz;
	else
		padSize = 0;

	character_ct = padSize + pbufsiz + 2;
	data_len = (character_ct + 1) * this->charSize();
	data = new uint8_t[data_len];

	// if (negative) this->initChar('-', 0);

	size_t pos = 0;

	this->initChar('0',pos++);
	this->initChar('x',pos++);

	for (size_t i=0; i<padSize; i++)
		this->initChar('0',pos++);

	for (size_t i=0; i<pbufsiz; i++)
		this->initChar(pbuf[pbufsiz-i-1], pos++);
}

template <encoding E>
string<E>::string(double value, int base, int precision, int padSize)
{
	uint8_t ibuf[Z_STR_INT_BUFSIZE];
	uint8_t fbuf[Z_STR_FLOAT_BUFSIZE];
	uint8_t ebuf[Z_STR_EXP_BUFSIZE];

	data = 0;

	union float_cast
	{
		double fval;
		unsigned long ival;

		struct
		{
			unsigned long mantissa : 52;
			unsigned int exponent : 11;
			bool sign : 1;
		};
	};
	float_cast number;
	number.fval = value;

	bool negative = number.sign;
	number.sign = 0;
	bool force = true;

	if ((base < 2) || (base > 36)) base = 10;
	if (precision <= 0)
	{
		precision = Z_STR_FLOAT_PRECISION;
		force = false;
	}

	//integral and fractional parts of the number
	unsigned long integral;
	double fractional;
	unsigned long exponent = 0;
	bool negexponent = false;

	if (number.ival) //don't bother with exponents if 0
	{
		double temp = number.fval;
		unsigned long tempExp = exponent;
		bool tempNegExp = negexponent;

		if (1023 <= number.exponent)// pos exponent
		{
			while (temp >= base)
			{
				temp /= base;
				tempExp++;
			}
		}
		else if (1023 >= number.exponent)// neg exponent
		{
			tempNegExp = true;
			double frac = 1.0 / (double)base;

			while (temp < frac)
			{
				temp *= base;
				tempExp++;
			}
		}

		if ((tempNegExp && (tempExp >= precision)) ||
			(number.fval > (double)(INT_MAX)))
		{
			number.fval = temp;
			exponent = tempExp;
			negexponent = tempNegExp;
		}
	}

	if (number.exponent < 1023)//x2^neg
	{
		integral = 0;
	}
	else if (number.exponent > 1023)//x2^(pos)
	{
		long expo = number.exponent - 1023;
		integral = ((long)1 << expo) + (number.mantissa >> ((long)52 - expo));
	}
	else //x2^0
	{
		integral = 1;
	}

	fractional = number.fval - (double)integral;

	//this section rounds to nearest fractional point (decimal pt. in base 10)
	if (number.ival)
	{
		//prep for rounding
		double tmp = fractional;
		double roundval = tmp / (double)(base << 1);
		double roundadd = roundval;
		for (size_t i=0; i<precision; i++)
		{
			tmp *= base;
			tmp -= (double)(long)tmp;
			roundadd /= base;
		}
		//round
		if (tmp >= roundval)
		{
			fractional += roundadd;

			if (fractional >= 1)
			{
				integral++;
				fractional--;
			}
		}
	}

	// number.exponent - 1023;
	size_t ibufsiz = integralBuf(integral, base, ibuf);
	size_t fbufsiz = fractionalBuf(fractional, base, precision, force, fbuf);
	// size_t fbufsiz = fractionalBuf(fractional, 10, 6, 0, fbuf);
	size_t ebufsiz = exponent ? integralBuf(exponent, base, ebuf) : 0;
	//initialize string data
	character_ct = ibufsiz + negative + (bool)fractional + fbufsiz + (bool)exponent + negexponent + ebufsiz;
	if (character_ct < padSize)
		character_ct += (padSize -= character_ct);
	else
		padSize = 0;

	// bool

	// character_ct = fbufsiz;

	// character_ct = fbufsiz + (bool)fbufsiz;
	data_len = (character_ct + 1) * this->charSize();
	// data_len = 100;
	// character_ct = 0;
	data = new uint8_t[data_len];
	// data = new uint8_t[10];
	// data_len = 10;
	// character_ct = 0;
	if (negative) this->initChar('-', 0);
	// size_t pos = 0;
	size_t pos = negative;
	//
	for (size_t i=0; i<padSize; i++)
		this->initChar('0',pos++);

	for (size_t i=0; i<ibufsiz; i++)
		this->initChar(ibuf[ibufsiz-i-1], pos++);

	if (fbufsiz)
	{
		this->initChar('.', pos++);

		for (size_t i=0; i<fbufsiz; i++)
			this->initChar(fbuf[i],pos++);
	}

	if (exponent)
	{
		this->initChar('e', pos++);
		if (negexponent)
			this->initChar('-', pos++);

		for (size_t i=0; i<ebufsiz; i++)
			this->initChar(ebuf[ebufsiz-i-1],pos++);
	}

	this->initChar(0,pos);
}

template <encoding E>
string<E>::string(const std::complex<double>& value, int base, int precision) : string()
{
	if (value.real() && value.imag())
	{
		operator=(string<E>(value.real(), base, precision, 0));
		if (value.imag() > 0) operator+=("+");
		operator+=(string<E>(value.imag(), base, precision, 0));
		operator+=("i");
	}
	else if (value.imag())
	{
		operator=(string<E>(value.imag(), base, precision, 0));
		operator+=("i");
	}
	else
	{
		operator=(string<E>(value.real(), base, precision, 0));
	}

}

template <encoding E>
string<E>::string(string<E>&& other)
{
	data = other.data;
	data_len = other.data_len;
	character_ct = other.character_ct;

	other.data = 0;
}

template <encoding E>
size_t string<E>::size() const
{
	return sizeof(*this) + (data_len * sizeof(uint8_t));
}

template <encoding E>
size_t string<E>::length() const
{
	return character_ct;
}

template <encoding E>
size_t string<E>::chars() const
{
	return character_ct;
}

template <encoding E>
void string<E>::increase(size_t goal)
{
	if (data_len >= goal) return;

	uint8_t* old_data = data;
	size_t old_data_len = data_len;

	//~1.5x string growth
	while (data_len < goal)
		data_len += (data_len >> 1) + 4;
	data = new uint8_t[data_len];

	size_t remain = old_data_len;
	uint32_t* data32 = (uint32_t*)data;
	uint32_t* old32 = (uint32_t*)old_data;

	size_t i = 0;

	//copy as much data as possible in 32-bit chunks
	while (remain > 4)
	{
		data32[i] = old32[i];

		i++;
		remain -= sizeof(uint32_t);
	}

	//copy remaining amount
	while (remain)
	{
		size_t offset = old_data_len - remain;
		data[offset] = old_data[offset];

		remain--;
	}
}

template <encoding E>
long string<E>::integer(int base) const
{
	if ((base < 2) || (base > 36)) return 0;

	bool negative = (data[0] == '-');
	long result = 0;

	size_t start = (negative || (data[0] == '+'));

	for (size_t i=start; i<character_ct; i++)
	{
		uint32_t chr = data[i];

		if (isNumeric(chr))
		{
			result *= base;
			result += numeralValue(chr);
		}
		else return 0;
	}

	return (negative ? -result : result);
}

template <encoding E>
double string<E>::floating(int base) const
{
	if ((base < 2) || (base > 36)) return 0;

	if (!character_ct) return 0;

	bool pastDecimal, pastExponent, negexponent;
	pastDecimal = pastExponent = negexponent = false;

	bool negative = (data[0] == '-');
	size_t start = (negative || (data[0] == '+'));

	if (start >= character_ct) return 0;

	double result = 0;
	double frac = 1;
	int exponent = 0;

	for (size_t i=start; i<character_ct; i++)
	{
		if (!isNumeric(data[i], base))
		{
			if (data[i] == '.')
			{
				if (pastDecimal || pastExponent)
					return 0;
				else
				{
					if (i >= character_ct-1) return false;
					pastDecimal = true;
				}
			}
			else if (toLower(data[i]) == 'e')
			{
				if (pastExponent)
					return 0;
				else
				{
					pastExponent = true;
					negexponent = (data[i+1] == '-');
					if (negexponent || (data[i+1] == '+'))
						i++;
				}
			}
			else return 0;
		}
		else
		{
			if (pastExponent)
			{
				exponent *= base;
				exponent += numeralValue(data[i]);
			}
			else if (pastDecimal)
			{
				frac /= base;
				result += (double)numeralValue(data[i])*frac;
			}
			else
			{
				result *= base;
				result += numeralValue(data[i]);
			}
		}
	}

	if (pastExponent)
	{
		for (int i=0; i<exponent; i++)
		{
			if (negexponent)
				result /= base;
			else
				result *= base;
		}
	}

	return (negative ? -result : result);
}

template <encoding E>
std::complex<double> string<E>::complex(int base) const
{
	if ((base < 2) || (base > 36)) return 0;

	if (!character_ct) return 0;

	bool pastDecimal, pastExponent, imag, ir, negexponent;
	pastDecimal = pastExponent = imag = ir = negexponent = false;

	bool negative = (data[0] == '-');
	size_t start = (negative || (data[0] == '+'));

	if (start >= character_ct) return 0;

	double imagResult = 0;
	double realResult = 0;
	double result = 0;
	double frac = 1;
	int exponent = 0;

	if (negative) result = -result;

	for (size_t i=start; i<character_ct; i++)
	{
		if (!isNumeric(data[i], base))
		{
			if (data[i] == '.')
			{
				if (pastDecimal || pastExponent)
					return 0;
				else
				{
					if ((i >= character_ct-1) || (toLower(data[i+1]) == 'i'))
						return 0;
					pastDecimal = true;
				}
			}
			else if (toLower(data[i]) == 'e')
			{
				if (pastExponent)
					return 0;
				else
				{
					pastExponent = true;

					negexponent = (data[i+1] == '-');
					if (negexponent || (data[i+1] == '+'))
						i++;
				}
			}
			else if (toLower(data[i]) == 'i')
			{
				if (imag)
					return 0;
				else
				{
					imag = true;

					if (!i || !isNumeric(data[i-1], base)) result = 1;

					if (pastExponent)
					{
						for (int i=0; i<exponent; i++)
						{
							if (negexponent)
								result /= base;
							else
								result *= base;
						}
					}

					pastExponent = pastDecimal = negexponent = false;

					imagResult = (negative ? -result : result);
					result = 0;
				}
			}
			else if ((data[i] == '-') || (data[i] == '+'))
			{
				if (ir || (i >= character_ct-1))
					return 0;
				else
				{
					ir = true;

					if (!imag)
					{
						if (pastExponent)
						{
							for (int i=0; i<exponent; i++)
							{
								if (negexponent)
									result /= base;
								else
									result *= base;
							}
						}

						realResult = (negative ? -result : result);
						result = 0;
					}

					pastDecimal = pastExponent = negexponent = false;

					negative = (data[i] == '-');
				}
			}
			else return 0;
		}
		else
		{
			if (pastExponent)
			{
				exponent *= base;
				exponent += numeralValue(data[i]);
			}
			else if (pastDecimal)
			{
				frac /= base;
				result += (double)numeralValue(data[i])*frac;
			}
			else
			{
				result *= base;
				result += numeralValue(data[i]);
			}
		}
	}

	if (pastExponent)
	{
		for (int i=0; i<exponent; i++)
		{
			if (negexponent)
				result /= base;
			else
				result *= base;
		}
	}

	if (result)
		realResult = (negative ? -result : result);

	return std::complex<double>(realResult, imagResult);
}

///analyzers
template <encoding E>
int string<E>::find(const string<E>& other, int occurrence) const
{
	return this->findAfter(other,0,occurrence);
}

template <encoding E>
int string<E>::findLast(const string<E>& other, int occurrence) const
{
	return this->findBefore(other, this->character_ct, occurrence);
}

template <encoding E>
bool string<E>::foundAt(const string<E>& other, size_t index) const
{
	if ((character_ct - index) < other.character_ct) return false;

	uint32_t* data32 = (uint32_t*)data;
	uint32_t* other32 = (uint32_t*)other.data;

	const size_t charSz = this->charSize();

	size_t i = 0;
	size_t idx = index * charSz;
	size_t end = (other.character_ct * charSz) >> 2;
	for (i=0; i<end; i++)
	{
		if (data32[i+index] != other32[i])
			return false;
	}

	for (i=(end<<2); i<(other.character_ct * charSz); i++)
	{
		if (data[i+idx] != other[i])
			return false;
	}

	return true;
}

template <encoding E>
bool string<E>::beginsWith(const string<E>& other) const
{
	return this->foundAt(other, 0);
}

template <encoding E>
bool string<E>::foundEndAt(const string<E>& other, size_t index) const
{
	if (index < other.character_ct) return false;
	if (index >= character_ct) return false;

	const size_t charSz = this->charSize();
	const size_t idx = (index-other.character_ct+1)*charSz;
	const size_t last = other.character_ct * charSz;

	uint32_t* data32 = (uint32_t*)&data[idx];
	uint32_t* other32 = (uint32_t*)other.data;

	size_t i = 0;
	size_t end = last >> 2;
	while (i < end)
	{
		if (data32[i] != other32[i])
			return false;

		i++;
	}

	for (i=(end<<2); i<last; i++)
	{
		if (data[i+idx] != other[i])
			return false;
	}

	return true;
}

template <encoding E>
bool string<E>::endsWith(const string<E>& other) const
{
	return this->foundEndAt(other, character_ct-1);
}

template <encoding E>
bool string<E>::isInteger(int base) const
{
	if ((base < 2) || (base > 36)) return false;

	if (!character_ct) return false;

	size_t start = ((data[0] == '-') || (data[0] == '+'));

	for (size_t i=start; i<character_ct; i++)
	{
		if (!isNumeric(data[i], base))
			return false;
	}

	return true;
}

template <encoding E>
bool string<E>::isFloating(int base) const
{
	if ((base < 2) || (base > 36)) return false;

	if (!character_ct) return false;

	bool pastDecimal, pastExponent;
	pastDecimal = pastExponent = false;

	size_t start = ((data[0] == '-') || (data[0] == '+'));

	if (start >= character_ct) return 0;

	for (size_t i=start; i<character_ct; i++)
	{
		if (!isNumeric(data[i], base))
		{
			if (data[i] == '.')
			{
				if (pastDecimal || pastExponent)
					return false;
				else
				{
					if (i >= character_ct-1) return false;
					pastDecimal = true;
				}
			}
			else if (toLower(data[i]) == 'e')
			{
				if (pastExponent)
					return false;
				else
				{
					pastExponent = true;
					if ((data[i+1] == '+') || (data[i+1] == '-'))
						i++;
				}
			}
			else return false;
		}
	}

	return true;
}

template <encoding E>
bool string<E>::isComplex(int base) const
{
	if ((base < 2) || (base > 36)) return false;

	if (!character_ct) return false;

	bool pastDecimal, pastExponent, imag, ir;
	pastDecimal = pastExponent = imag = ir = false;

	size_t start = ((data[0] == '-') || (data[0] == '+'));

	if (start >= character_ct) return 0;

	for (size_t i=start; i<character_ct; i++)
	{
		if (!isNumeric(data[i], 10))
		{
			if (data[i] == '.')
			{
				if (pastDecimal || pastExponent)
					return false;
				else
				{
					if ((i >= character_ct-1) || (toLower(data[i+1]) == 'i'))
						return false;
					pastDecimal = true;
				}
			}
			else if (toLower(data[i]) == 'e')
			{
				if (pastExponent)
					return false;
				else
				{
					pastExponent = true;
					if ((data[i+1] == '+') || (data[i+1] == '-'))
						i++;
				}
			}
			else if (toLower(data[i]) == 'i')
			{
				if (imag)
					return false;
				else
				{
					pastExponent = pastDecimal = false;
					imag = true;
				}
			}
			else if ((data[i] == '-') || (data[i] == '+'))
			{
				if (ir || (i >= character_ct-1))
					return false;
				else
				{
					pastDecimal = pastExponent = false;
					ir = true;
				}
			}
			else return false;
		}
	}

	return true;
}

///mutators
template <encoding E>
const string<E>& string<E>::remove(const string& other, int occurrence)
{
	if (occurrence > 0) //remove one occurrence
	{
		int pos = this->find(other, occurrence);

		if (pos >= 0)
		{
			this->remove(pos, other.length());
		}
	}
	else if (!occurrence) //remove all occurrences
	{
		int pos = this->find(other, 1);

		while (pos >= 0)
		{
			this->remove(pos, other.length());
			pos = this->findAfter(other, pos, 1);
		}
	}

	return *this;
}

template <encoding E>
const string<E>& string<E>::append(const string<E>& other)
{
	return operator+=(other);
}

template <encoding E>
const string<E>& string<E>::replace(const string<E>& findStr, const string<E>& replStr, int occurrence)
{
	if (occurrence > 0) //replace one occurrence
	{
		int pos = this->find(findStr, occurrence);

		if (pos >= 0)
		{
			this->replace((size_t)pos, (int)findStr.length(), replStr);
		}
	}
	else if (!occurrence) //replace all occurrences
	{
		int pos = this->find(findStr, 1);

		while (pos >= 0)
		{
			this->replace((size_t)pos, (int)findStr.length(), replStr);
			pos = this->findAfter(findStr, pos+replStr.length(), 1);
		}
	}

	return *this;
}

template <encoding E>
const string<E>& string<E>::padLeft(const string<E>& other, size_t padSize)
{
	if (padSize <= character_ct) return *this;

	string<E> padStr;

	size_t padChars = padSize - character_ct;

	while (padChars >= other.character_ct)
	{
		padStr += other;
		padChars -= other.character_ct;
	}

	if (padChars)
		padStr += other.substr(0, padChars);

	return this->insert(padStr, 0);
}

template <encoding E>
const string<E>& string<E>::padRight(const string<E>& other, size_t padSize)
{
	if (padSize <= character_ct) return *this;

	string<E> padStr;

	size_t padChars = padSize - character_ct;

	while (padChars >= other.character_ct)
	{
		padStr += other;
		padChars -= other.character_ct;
	}

	if (padChars)
		padStr += other.substr(0, padChars);

	return operator+=(padStr);
}

template <encoding E>
const string<E>& string<E>::unPadLeft(const string<E>& other)
{
	if (character_ct < other.character_ct) return *this;

	size_t index = 0;
	while (this->foundAt(other, index))
	{
		index += other.character_ct;
	}

	return this->remove(0, index);
}

template <encoding E>
const string<E>& string<E>::unPadRight(const string<E>& other)
{
	if (character_ct < other.character_ct) return *this;

	size_t index = character_ct-other.character_ct;
	int count = 0;
	while (this->foundAt(other, index))
	{
		index -= other.character_ct;
		count += other.character_ct;
	}

	return this->remove(index+other.character_ct, character_ct);
}

template <encoding E>
const string<E>& string<E>::unPad(const string<E>& other)
{
	this->unPadLeft(other);
	return this->unPadRight(other);
}

template <encoding E>
const string<E>& string<E>::cutDuplicates(const string<E>& other)
{
	int pos = this->find(other, 1);

	while (pos >= 0)
	{
		size_t opos = pos + other.length();
		while (this->foundAt(other, opos))
		{
			this->remove(opos, other.length());
		}
		pos = this->findAfter(other, opos, 1);
	}

	return *this;
}

///operators
template <encoding E>
string<E> string<E>::operator+(const string<E>& other) const
{
	string<E> result (*this);
	result += other;

	return result;
}

template <encoding E>
const string<E>& string<E>::operator=(const string<E>& other)
{
	size_t new_len = (other.character_ct + 1) * this->charSize();
	this->increase(new_len);
	// data = new uint8_t[data_len];

	character_ct = other.character_ct;

	uint32_t* data32 = (uint32_t*)data;
	uint32_t* other32 = (uint32_t*)other.data;
	size_t len32 = data_len >> 2;

	for (size_t i=0; i<len32; i++)
		data32[i] = other32[i];

	size_t len = len32 << 2;
	for (size_t i=len; i<data_len; i++)
		data[i] = other.data[i];

	return *this;
}

template <encoding E>
bool string<E>::operator==(const string<E>& other) const
{
	if (character_ct != other.character_ct)
		return false;

	uint32_t* data32 = (uint32_t*)data;
	uint32_t* other32 = (uint32_t*)other.data;
	size_t len32 = (character_ct * this->charSize()) >> 2;

	for (size_t i=0; i<len32; i++)
	{
		if (data32[i] != other32[i])
			return false;
	}

	size_t len = len32 << 2;
	size_t max = character_ct * this->charSize();
	for (size_t i=len; i<max; i++)
	{
		if (data[i] != other.data[i]);
			return false;
	}

	return true;
}

template <encoding E>
bool string<E>::operator!=(const string<E>& other) const
{
	return !operator==(other);
}

template <encoding E>
bool string<E>::operator>(const string<E>& other) const
{
	size_t max_char;
	if (character_ct < other.character_ct)
		max_char = character_ct;
	else
		max_char = other.character_ct;

	uint32_t* data32 = (uint32_t*)data;
	uint32_t* other32 = (uint32_t*)other.data;
	size_t len32 = (max_char * this->charSize()) >> 2;

	for (size_t i=0; i<len32; i++)
	{
		if (data32[i] > other32[i])
			return true;
	}

	size_t len = len32 << 2;
	size_t max = max_char * this->charSize();
	for (size_t i=len; i<max; i++)
	{
		if (data[i] > other.data[i]);
			return true;
	}

	if (character_ct > other.character_ct)
		return true;

	return false;
}

template <encoding E>
bool string<E>::operator>=(const string<E>& other) const
{
	return !operator<(other);
}

template <encoding E>
bool string<E>::operator<(const string<E>& other) const
{
	size_t max_char;
	if (character_ct < other.character_ct)
		max_char = character_ct;
	else
		max_char = other.character_ct;

	uint32_t* data32 = (uint32_t*)data;
	uint32_t* other32 = (uint32_t*)other.data;
	size_t len32 = (max_char * this->charSize()) >> 2;

	for (size_t i=0; i<len32; i++)
	{
		if (data32[i] < other32[i])
			return true;
	}

	size_t len = len32 << 2;
	size_t max = max_char * this->charSize();
	for (size_t i=len; i<max; i++)
	{
		if (data[i] < other.data[i]);
			return true;
	}

	if (character_ct < other.character_ct)
		return true;

	return false;
}

template <encoding E>
bool string<E>::operator<=(const string<E>& other) const
{
	return !operator>(other);
}

template <encoding E>
void string<E>::read(inputStream& stream, uint32_t delim)
{
	character_ct = 0;
	this->increase(1);

	if (stream.bad())
	{
		data[character_ct] = 0;
		return;
	}

	uint32_t last = stream.getChar(E);

	while (!stream.empty() && (delim ? (last == delim) : isWhiteSpace(last)))
		last = stream.getChar(E);

	while (!stream.empty() && !(delim ? (last == delim) : isWhiteSpace(last)))
	{
		data[character_ct++] = last;
		this->increase(character_ct+1);

		last = stream.getChar(E);
	}

	data[character_ct] = 0;
}

template <encoding E>
void string<E>::readln(inputStream& stream)
{
	character_ct = 0;
	this->increase(1);

	if (stream.bad())
	{
		data[character_ct] = 0;
		return;
	}

	uint32_t last = stream.getChar(E);

	while (!stream.empty())
	{
		if (last == '\r')
		{
			last = stream.getChar(E);
			if (last == '\n')
			{
				data[character_ct] = 0;
				return;
			}
			data[character_ct++] = '\r';
		}
		else if (last == '\n')
		{
			data[character_ct] = 0;
			return;
		}

		data[character_ct++] = last;
		this->increase(character_ct+1);

		last = stream.getChar(E);
	}

	data[character_ct] = 0;
}

template <encoding E>
void string<E>::write(outputStream& stream) const
{
	if (stream.bad()) return;

	if (character_ct)
		stream.put(data, character_ct, E);
}

template <encoding E>
void string<E>::writeln(outputStream& stream) const
{
	if (stream.bad()) return;

	if (character_ct)
		stream.put(data, character_ct, E);

	string<E> newl = "\n";
	stream.put(newl.data, newl.character_ct, E);
}

template <encoding E>
void string<E>::serialIn(inputStream& stream)
{
	if (stream.bad() || !stream.binary())
	{
		character_ct = 0;
		this->increase(4);
		*((uint32_t*)data) = 0;
		return;
	}

	uint8_t c[sizeof(size_t)];
	for (size_t i=0; i<sizeof(size_t); i++)
		c[i] = stream.get();

	size_t datact = *((size_t*)c);
	character_ct = datact / this->charSize();
	this->increase(datact + 4);

	size_t i = 0;
	while (!stream.empty() && (i < datact))
	{
		data[i++] = stream.get();
	}

	*((uint32_t*)&data[i]) = 0;
}

template <encoding E>
void string<E>::serialOut(outputStream& stream) const
{
	if (stream.bad() || !stream.binary())
		return;

	size_t datact = character_ct * this->charSize();
	uint8_t* c = (uint8_t*)&datact;
	for (size_t i=0; i<sizeof(size_t); i++)
		stream.put(c[i]);

	size_t i = 0;
	while ((i < datact))
	{
		stream.put(data[i++]);
	}
}
