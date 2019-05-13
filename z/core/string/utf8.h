#pragma once

namespace z
{
	namespace core
	{
		template <>
		void string<utf8>::increase(size_t max_chars)
		{
			size_t goal = max_chars + 1; //account for null byte at the end
			if (data_len >= goal) return;

			uint8_t* old_data = data;
			size_t old_data_len = data_len;

			//~1.5x string growth
			while (data_len < goal)
				data_len += (data_len + 4) >> 1;
			data = new uint8_t[data_len];

			size_t remain = old_data_len;
			uint32_t* data32 = (uint32_t*)data;
			uint32_t* old32 = (uint32_t*)old_data;

			size_t i = 0;

			//copy as much data as possible in 32-bit chunks
			while (remain >= 4)
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

			delete[] old_data;
		}

		template <>
		const uint8_t* string<utf8>::cstring() const
		{
			return data;
		}

		template <>
		const uint16_t* string<utf8>::nstring() const
		{
			return 0;
		}

		template <>
		const uint32_t* string<utf8>::wstring() const
		{
			return 0;
		}

		template <>
		void string<utf8>::initChar(uint32_t chr, size_t index)
		{
			data[index] = chr;
		}

		template <>
		size_t string<utf8>::charSize() const
		{
			return 1;
		}

		template <>
		size_t string<utf8>::chars() const
		{
			size_t i = 0;
			size_t result = 0;

			while (i < character_ct)
			{
				i += lenFromUTF8(&data[i]);
				result++;
			}

			return result;
		}

		template <>
		uint32_t string<utf8>::at(size_t index) const
		{
			if (index < character_ct)
				return fromUTF8(&data[index]);
			else
				return 0;
		}

		///operators
		template <>
		const string<utf8>& string<utf8>::operator+=(const string<utf8>& other)
		{
			size_t new_size = character_ct + other.character_ct + 1;
			this->increase(new_size);

			for (size_t i=0; i<other.character_ct; i++)
			{
				data[character_ct + i] = other.data[i];
			}
			data[new_size-1] = 0;

			character_ct += other.character_ct;

			return *this;
		}

		///analyzers
		template <>
		int string<utf8>::count(const string<utf8>& other) const
		{
			if (!other.character_ct) return 0;

			size_t occurrence = 0;

			size_t other_i = 0;
			for (size_t i=0; i<character_ct; i++)
			{
				if (data[i] == other.data[other_i])
				{
					other_i++;
					if (other_i >= other.character_ct)
						occurrence++;

					if (!occurrence) return (i - other_i + 1);
				}
				else
				{
					if ((character_ct - i) <= other.character_ct)
						return occurrence;

					other_i = 0;
				}
			}

			return occurrence;
		}

		template <>
		int string<utf8>::findAfter(const string<utf8>& other, size_t index, int occurrence) const
		{
			if (!other.character_ct || (occurrence < 1)) return -1;

			size_t other_i = 0;
			for (size_t i=index; i<character_ct; i++)
			{
				//reset to first char of other if not still matching
				if (data[i] != other.data[other_i])
					other_i = 0;

				if (data[i] == other.data[other_i])
				{
					other_i++;
					if (other_i >= other.character_ct)
					{
						occurrence--;
						other_i = 0;
					}

					if (!occurrence) return (i - other.character_ct + 1);
				}
			}

			return -1;
		}

		template <>
		int string<utf8>::findBefore(const string<utf8>& other, size_t index, int occurrence) const
		{
			if (!other.character_ct || (occurrence < 1)) return -1;

			if (index > (character_ct - other.character_ct))
				index = character_ct - other.character_ct;

			size_t other_i = other.character_ct - 1;
			for (size_t i=index; i<character_ct; i--)
			{
				if (data[i] == other.data[other_i])
				{

					if (!other_i)
					{
						occurrence--;

						if (occurrence && (i < other.character_ct)) i = 0;
						other_i = other.character_ct - 1;
					}
					else
						other_i--;

					if (!occurrence) return i;
				}
				else
				{
					if (occurrence && (i < other.character_ct)) return -1;
					other_i = other.character_ct - 1;
				}
			}

			return -1;
		}

		///mutators
		template <>
		const string<utf8>& string<utf8>::operator=(const string<utf8>& other)
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

		template <>
		string<utf8> string<utf8>::substr(size_t index, int count) const
		{
			return string<utf8>(string<utf32>(*this).substr(index,count));
		}

		template <>
		const string<utf8>& string<utf8>::insert(const string<utf8>& other, size_t index)//insert before index
		{
			if (!other.character_ct) return *this;

			if (index >= character_ct) index = character_ct;

			size_t start = index + other.character_ct;
			size_t end = character_ct + other.character_ct;
			this->increase(end);

			for (size_t i=end-1; i>=start; i--)
			{
				data[i] = data[i - other.character_ct];
			}

			end = index + other.character_ct;
			for (size_t i=index; i<end; i++)
			{
				data[i] = other.data[i-index];
			}

			character_ct += other.character_ct;
			data[character_ct] = 0;

			return *this;
		}

		template <>
		const string<utf8>& string<utf8>::remove(size_t index, int count)
		{
			if (count)
			{
				if (index >= character_ct) return *this;

				size_t start, end, offset;

				if (count < 0)
				{
					if ((index + count) > character_ct)
					{
						start = 0;
						offset = index;
					}
					else
					{
						start = index + count;
						offset = -count;
					}
				}
				else
				{
					if ((index + count) > character_ct)
						offset = character_ct - index;
					else
						offset = count;

					start = index;
				}


				end = character_ct - offset;
				for (size_t i=start; i<end; i++)
				{
					data[i] = data[i+offset];
				}

				data[end] = 0;
				character_ct = end;
			}

			return *this;
		}

		template <>
		const string<utf8>& string<utf8>::replace(size_t index, int count, const string<utf8>& other)
		{
			if (count)
			{
				size_t start, end;

				if (count < 0)
				{
					if ((index >= character_ct) && ((size_t)-count >= character_ct))
						return operator=(other);

					if (index >= character_ct)
					{
						start = character_ct + count;
						end = character_ct;
					}
					else
					{
						end = index + 1;

						if ((size_t)-count >= character_ct)
							start = 0;
						else
							start = end + count;
					}
				}
				else
				{
					if (index >= character_ct)
						return operator+=(other);

					if (!index && ((size_t)count >= character_ct))
						return operator=(other);

					start = index;
					if ((size_t)count >= character_ct)
						end = character_ct;
					else
						end = start + count;
				}


				size_t offset = end - start;
				size_t newCharCt = character_ct - offset + other.character_ct;
				this->increase(newCharCt + 1);

				if (newCharCt < character_ct)
				{
					//pull chars in
					size_t toOffs = newCharCt - character_ct;

					for (size_t i=end; i<character_ct; i++)
						data[i+toOffs] = data[i];
				}
				else if (newCharCt > character_ct)
				{
					//pull chars out
					size_t toPos = newCharCt + 1;
					size_t fromPos = character_ct + 1;

					for (size_t i=end; i<character_ct; i++)
						data[toPos-i] = data[fromPos-i];
				}
				//else just directly replace chars

				for (size_t i=0; i<other.character_ct; i++)
					data[i+start] = other.data[i];

				character_ct = newCharCt;
				data[character_ct] = 0;
			}

			return *this;
		}

		template <>
		uint32_t string<utf8>::operator[](size_t index) const
		{
			return this->at(index);
		}

		template <>
		void string<utf8>::initInt(long long value, unsigned int base, unsigned int padSize)
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

		template <>
		void string<utf8>::initPointer(void* pointer)
		{
			uint8_t pbuf[Z_STR_INT_BUFSIZE];

			union ptv
			{
				void* pval;
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

		template <>
		void string<utf8>::initFloat(double value, unsigned int base, unsigned int precision, unsigned int padSize)
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
				} raw;
			};
			float_cast number;
			number.fval = value;

			bool negative = number.raw.sign;
			number.raw.sign = 0;
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

				if (1023 <= number.raw.exponent)// pos exponent
				{
					while (temp >= base)
					{
						temp /= base;
						tempExp++;
					}
				}
				else if (1023 >= number.raw.exponent)// neg exponent
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

			if (number.raw.exponent < 1023)//x2^neg
			{
				integral = 0;
			}
			else if (number.raw.exponent > 1023)//x2^(pos)
			{
				long expo = number.raw.exponent - 1023;
				integral = ((long)1 << expo) + (number.raw.mantissa >> ((long)52 - expo));
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

			// number.raw.exponent - 1023;
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

		template <>
		void string<utf8>::initComplex(const std::complex<double>& value, unsigned int base, unsigned int precision)
		{
			data_len = 4;
			data = new uint8_t[data_len];
			*((uint32_t*)data) = 0;
			character_ct = 0;

			if (value.real() && value.imag())
			{
				operator=(string<utf32>(value.real(), base, precision, 0));
				if (value.imag() > 0) operator+=("+");
				operator+=(string<utf32>(value.imag(), base, precision, 0));
				operator+=("i");
			}
			else if (value.imag())
			{
				operator=(string<utf32>(value.imag(), base, precision, 0));
				operator+=("i");
			}
			else
			{
				operator=(string<utf32>(value.real(), base, precision, 0));
			}

		}

		template <>
		size_t string<utf8>::size() const
		{
			return sizeof(*this) + (data_len * sizeof(uint8_t));
		}

		template <>
		size_t string<utf8>::length() const
		{
			return character_ct;
		}

		template <>
		long string<utf8>::integer(int base) const
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

		template <>
		double string<utf8>::floating(int base) const
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

		template <>
		std::complex<double> string<utf8>::complex(int base) const
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

		template <>
		int string<utf8>::type(int base, uint32_t decimal) const
		{
			if ((base < 2) || (base > 36) || !character_ct) return zstr::string;

			bool pastDecimal, pastExponent, imag, ir;
			pastDecimal = pastExponent = imag = ir = false;

			size_t start = ((data[0] == '-') || (data[0] == '+'));

			if (start >= character_ct) return zstr::string;

			for (size_t i=start; i<character_ct; i++)
			{
				if (!isNumeric(data[i], 10))
				{
					if (data[i] == decimal)
					{
						if (pastDecimal || pastExponent)
							return zstr::string;
						else
						{
							if ((i >= character_ct-1) || (toLower(data[i+1]) == 'i'))
								return zstr::string;
							pastDecimal = true;
						}
					}
					else if (toLower(data[i]) == 'e')
					{
						if (pastExponent)
							return zstr::string;
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
							return zstr::string;
						else
						{
							pastExponent = pastDecimal = false;
							imag = true;
						}
					}
					else if ((data[i] == '-') || (data[i] == '+'))
					{
						if (ir || (i >= character_ct-1))
							return zstr::string;
						else
						{
							pastDecimal = pastExponent = false;
							ir = true;
						}
					}
					else return zstr::string;
				}
			}

			if (imag)
			{
				return zstr::complex;
			}
			else
			{
				return pastDecimal ? zstr::floating : zstr::integer;
			}
		}

		///analyzers
		template <>
		bool string<utf8>::foundAt(const string<utf8>& other, size_t index) const
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

		template <>
		bool string<utf8>::foundEndAt(const string<utf8>& other, size_t index) const
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

		///mutators
		template <>
		const string<utf8>& string<utf8>::remove(const string& other, int occurrence)
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

		template <>
		const string<utf8>& string<utf8>::replace(const string<utf8>& findStr, const string<utf8>& replStr, int occurrence)
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

		template <>
		const string<utf8>& string<utf8>::padLeft(const string<utf8>& other, size_t padSize)
		{
			if (padSize <= character_ct) return *this;

			string<utf8> padStr;

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

		template <>
		const string<utf8>& string<utf8>::padRight(const string<utf8>& other, size_t padSize)
		{
			if (padSize <= character_ct) return *this;

			string<utf8> padStr;

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

		template <>
		const string<utf8>& string<utf8>::trimLeft(const string<utf8>& other)
		{
			if (character_ct < other.character_ct) return *this;

			size_t index = 0;
			while (this->foundAt(other, index))
			{
				index += other.character_ct;
			}

			return this->remove(0, index);
		}

		template <>
		const string<utf8>& string<utf8>::trimRight(const string<utf8>& other)
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

		template <>
		const string<utf8>& string<utf8>::cutDuplicates(const string<utf8>& other)
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

		template <>
		string<utf8> string<utf8>::upper() const
		{
			string<utf8> result;

			uint8_t buf[4];
			size_t i=0;
			while (i < character_ct)
			{
				auto ch = toUpper(fromUTF8(&data[i]));

				int len = toUTF8(buf, ch);
				result.increase(result.character_ct + len + 1);

				for (int k=0; k<len; k++)
					result.data[result.character_ct++] = buf[k];
				result.data[result.character_ct] = 0;

				i += lenFromUTF8(&data[i]);
			}

			return result;
		}

		template <>
		string<utf8> string<utf8>::lower() const
		{
			string<utf8> result;

			uint8_t buf[4];
			size_t i=0;
			while (i < character_ct)
			{
				auto ch = toLower(fromUTF8(&data[i]));

				int len = toUTF8(buf, ch);
				result.increase(result.character_ct + len + 1);

				for (int k=0; k<len; k++)
					result.data[result.character_ct++] = buf[k];
				result.data[result.character_ct] = 0;

				i += lenFromUTF8(&data[i]);
			}

			return result;
		}

		template <>
		string<utf8> string<utf8>::camel() const
		{
			string<utf8> result;

			bool doLower = false;

			uint8_t buf[4];
			size_t i=0;
			while (i < character_ct)
			{
				auto ch = fromUTF8(&data[i]);
				ch = (doLower ? toLower(ch) : toUpper(ch, true));

				int len = toUTF8(buf, ch);
				result.increase(result.character_ct + len + 1);

				for (int k=0; k<len; k++)
					result.data[result.character_ct++] = buf[k];
				result.data[result.character_ct] = 0;

				doLower = isAlphaNumeric(ch) || (ch == '_');
				i += lenFromUTF8(&data[i]);
			}

			return result;
		}

		///operators
		template <>
		string<utf8> string<utf8>::operator+(const string<utf8>& other) const
		{
			string<utf8> result (*this);
			result += other;

			return result;
		}

		template <>
		bool string<utf8>::operator==(const string<utf8>& other) const
		{
			if (character_ct != other.character_ct)
				return false;

			size_t fast_sz = character_ct / sizeof(size_t);
			size_t slow_bg = fast_sz * sizeof(size_t);

			size_t* data_ptr = (size_t*)data;
			size_t* other_ptr = (size_t*)other.data;

			//compare max number of bytes at a time (usu. 8)
			//Minor slowdown for small strings, faster for large strings.
			for (size_t i=0; i<fast_sz; i++)
			{
				if (data_ptr[i] != other_ptr[i])
					return false;
			}

			//check any remaining characters
			for (size_t i=slow_bg; i<character_ct; i++)
			{
				if (data[i] != other.data[i])
					return false;
			}

			return true;
		}

		template <>
		bool string<utf8>::operator>(const string<utf8>& other) const
		{
			size_t max_char;
			if (character_ct < other.character_ct)
				max_char = character_ct;
			else
				max_char = other.character_ct;

			size_t fast_sz = max_char / sizeof(size_t);
			size_t slow_bg = fast_sz * sizeof(size_t);

			size_t* data_ptr = (size_t*)data;
			size_t* other_ptr = (size_t*)other.data;

			//compare max number of bytes at a time (usu. 8)
			//Minor slowdown for small strings, faster for large strings.
			for (size_t i=0; i<fast_sz; i++)
			{
				if (data_ptr[i] > other_ptr[i])
					return true;
				else if (data_ptr[i] < other_ptr[i])
					return false;
			}

			//check any remaining characters
			for (size_t i=slow_bg; i<max_char; i++)
			{
				if (data[i] > other.data[i])
					return true;
				else if (data[i] < other.data[i])
					return false;
			}

			return (character_ct > other.character_ct);
		}

		template <>
		bool string<utf8>::operator<(const string<utf8>& other) const
		{
			size_t max_char;
			if (character_ct < other.character_ct)
				max_char = character_ct;
			else
				max_char = other.character_ct;

			size_t fast_sz = max_char / sizeof(size_t);
			size_t slow_bg = fast_sz * sizeof(size_t);

			size_t* data_ptr = (size_t*)data;
			size_t* other_ptr = (size_t*)other.data;

			//compare max number of bytes at a time (usu. 8)
			//Minor slowdown for small strings, faster for large strings.
			for (size_t i=0; i<fast_sz; i++)
			{
				if (data_ptr[i] < other_ptr[i])
					return true;
				else if (data_ptr[i] > other_ptr[i])
					return false;
			}

			//check any remaining characters
			for (size_t i=slow_bg; i<max_char; i++)
			{
				if (data[i] < other.data[i])
					return true;
				else if (data[i] < other.data[i])
					return false;
			}

			return (character_ct < other.character_ct);
		}

		template <>
		void string<utf8>::read(inputStream& stream, uint32_t delim)
		{
			character_ct = 0;
			data[0] = 0;

			if (stream.bad() || stream.empty()) return;

			encoding enc = stream.format();
			uint32_t last = stream.getChar(enc);

			while (!stream.empty() && (delim ? (last == delim) : isWhiteSpace(last)))
				last = stream.getChar(enc);

			while (!stream.empty() && !(delim ? (last == delim) : isWhiteSpace(last)))
			{
				if (enc == utf8)
				{
					increase(character_ct);
					data[character_ct++] = last;
				}
				else
				{
					uint8_t c[4];
					int len = toUTF8(c, last);
					increase(character_ct+len);

					for (int i=0; i<len; i++)
						data[character_ct+i] = c[i];

					character_ct += len;
				}

				last = stream.getChar(enc);
			}

			increase(character_ct);
			data[character_ct] = 0;
		}

		template <>
		void string<utf8>::readln(inputStream& stream)
		{
			character_ct = 0;
			data[0] = 0;

			if (stream.bad() || stream.empty()) return;

			encoding enc = stream.format();
			uint32_t last = stream.getChar(utf8);

			while (!stream.empty())
			{
				if (stream.seekable())
				{
					if (last == '\r')
					{
						auto pos = stream.tell();
						last = stream.getChar(enc);
						if (!stream.empty() && (last != '\n'))
						{
							stream.seek(pos);
						}
						break;
					}
					else if (last == '\n')
					{
						auto pos = stream.tell();
						last = stream.getChar(enc);
						if (!stream.empty() && (last != '\r'))
						{
							stream.seek(pos);
						}
						break;
					}
				}
				else
				{
					if ((last == '\n') || (last == '\r')) break;
				}

				if (enc == utf8)
				{
					increase(character_ct);
					data[character_ct++] = last;
				}
				else
				{
					uint8_t c[4];
					int len = toUTF8(c, last);
					increase(character_ct + len);
					for (int i=0; i<len; i++)
						data[character_ct+i] = c[i];

					character_ct += len;
				}

				last = stream.getChar(enc);
			}

			increase(character_ct);
			data[character_ct] = 0;
		}

		template <>
		void string<utf8>::write(outputStream& stream, encoding enc) const
		{
			if (stream.bad()) return;

			if (character_ct)
			{
				if (enc == utf8)
				{
					stream.put(data, character_ct, enc);
				}
				else if (enc == utf16)
				{
					string<utf16> temp (*this);
					stream.put(temp.data, temp.character_ct, enc);
				}
				else if (enc == utf32)
				{
					string<utf32> temp (*this);
					stream.put(temp.data, temp.character_ct, enc);
				}
				else
				{
					string<ascii> temp (*this);
					stream.put(temp.data, temp.character_ct, enc);
				}
			}
		}

		template <>
		void string<utf8>::writeln(outputStream& stream, encoding enc) const
		{
			if (stream.bad()) return;
			string<utf8> newl = "\n";

			this->write(stream, enc);
			newl.write(stream, enc);
		}

		template <>
		void string<utf8>::serialIn(inputStream& stream)
		{
			if (stream.bad() || !stream.binary())
			{
				character_ct = 0;
				data[0] = 0;
				return;
			}

			core::serialIn(character_ct, stream);
			increase(character_ct);

			size_t i = 0;
			while (!stream.empty() && (i < character_ct))
			{
				data[i++] = stream.get();
			}

			data[i] = 0;
		}

		template <>
		void string<utf8>::serialOut(outputStream& stream) const
		{
			if (stream.bad() || !stream.binary())
				return;

			size_t datact = character_ct;
			core::serialOut(datact, stream);

			stream.put(data, character_ct, utf8);
		}

	} //end of core namespace
} //end of z namespace
