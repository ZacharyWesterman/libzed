#pragma once

namespace z
{
	namespace core
	{
		template <>
		void string<utf8>::increase(int max_chars) noexcept
		{
			int goal = max_chars + 1; //account for null byte at the end
			if (data_len >= goal) return;

			uint8_t* old_data = data;
			int old_data_len = data_len;

			//~1.5x string growth
			while (data_len < goal)
				data_len += (data_len + 4) >> 1;
			data = new uint8_t[data_len];

			int remain = old_data_len;
			uint32_t* data32 = (uint32_t*)data;
			uint32_t* old32 = (uint32_t*)old_data;

			int i = 0;

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
				int offset = old_data_len - remain;
				data[offset] = old_data[offset];

				remain--;
			}

			delete[] old_data;
		}

		template <>
		const char* string<utf8>::cstring() const noexcept
		{
			return (char*)data;
		}

		template <>
		const uint16_t* string<utf8>::nstring() const noexcept
		{
			return 0;
		}

		template <>
		const uint32_t* string<utf8>::wstring() const noexcept
		{
			return 0;
		}

		template <>
		void string<utf8>::initChar(uint32_t chr, int index) noexcept
		{
			data[index] = chr;
		}

		template <>
		int string<utf8>::charSize() const noexcept
		{
			return 1;
		}

		template <>
		int string<utf8>::chars() const noexcept
		{
			int i = 0;
			int result = 0;

			while (i < character_ct)
			{
				i += lenFromUTF8(&data[i]);
				result++;
			}

			return result;
		}

		template <>
		uint32_t string<utf8>::at(int index) const noexcept
		{
			if (index < 0) index += character_ct;
			if ((index < character_ct) && (index >= 0))
				return fromUTF8(&data[index]);
			else
				return 0;
		}

		///operators
		template <>
		string<utf8>& string<utf8>::operator+=(const string<utf8>& other) noexcept
		{
			int new_size = character_ct + other.character_ct + 1;
			this->increase(new_size);

			for (int i=0; i<other.character_ct; i++)
			{
				data[character_ct + i] = other.data[i];
			}
			data[new_size-1] = 0;

			character_ct += other.character_ct;

			return *this;
		}

		///analyzers
		template <>
		int string<utf8>::count(const string<utf8>& other) const noexcept
		{
			if (!other.character_ct) return 0;

			int occurrence = 0;

			int other_i = 0;
			for (int i=0; i<character_ct; i++)
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
		int string<utf8>::findAfter(const string<utf8>& other, int index, int occurrence) const noexcept
		{
			if (index < 0) index += character_ct;
			if (!other.character_ct || (occurrence < 1) || (index < 0)) return -1;

			int other_i = 0;
			for (int i=index; i<character_ct; i++)
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
		int string<utf8>::findBefore(const string<utf8>& other, int index, int occurrence) const noexcept
		{
			if (index < 0) index += character_ct;
			if (!other.character_ct || (occurrence < 1) || (index < 0)) return -1;

			if (index > (character_ct - other.character_ct))
				index = character_ct - other.character_ct;

			int other_i = other.character_ct - 1;
			for (int i=index; i<character_ct; i--)
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
		string<utf8>& string<utf8>::operator=(const string<utf8>& other) noexcept
		{
			if (&other != this)
			{
				int new_len = (other.character_ct + 1) * this->charSize();
				this->increase(new_len);
				// data = new uint8_t[data_len];

				character_ct = other.character_ct;

				uint32_t* data32 = (uint32_t*)data;
				uint32_t* other32 = (uint32_t*)other.data;
				int len32 = data_len >> 2;

				for (int i=0; i<len32; i++)
					data32[i] = other32[i];

				int len = len32 << 2;
				for (int i=len; i<data_len; i++)
					data[i] = other.data[i];
			}

			return *this;
		}

		template<>
		string<utf8>& string<utf8>::append(uint32_t chr) noexcept
		{
			if (chr)
			{
				uint8_t c[4];
				int len = toUTF8(c, chr);
				increase(character_ct+len);

				for (int i=0; i<len; ++i)
					data[character_ct++] = c[i];

				data[character_ct] = 0;
			}
			return *this;
		}

		template <>
		string<utf8>& string<utf8>::insert(const string<utf8>& other, int index) noexcept //insert before index
		{
			if (index < 0) index += character_ct;
			if (!other.character_ct || index < 0) return *this;

			if (index >= character_ct) index = character_ct;

			int start = index + other.character_ct;
			int end = character_ct + other.character_ct;
			increase(end);

			for (int i=end-1; i>=start; i--)
			{
				data[i] = data[i - other.character_ct];
			}

			end = index + other.character_ct;
			for (int i=index; i<end; i++)
			{
				data[i] = other.data[i-index];
			}

			character_ct += other.character_ct;
			data[character_ct] = 0;

			return *this;
		}

		template <>
		string<utf8>& string<utf8>::remove(int index, int count) noexcept
		{
			if (count)
			{
				if (index < 0) index += character_ct;
				if (index >= character_ct || index < 0) return *this;

				int start, end, offset;

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
				for (int i=start; i<end; i++)
				{
					data[i] = data[i+offset];
				}

				data[end] = 0;
				character_ct = end;
			}

			return *this;
		}

		template <>
		string<utf8>& string<utf8>::replace(int index, int count, const string<utf8>& other) noexcept
		{
			if (count)
			{
				if (index < 0) index += character_ct;
				if (index < 0) index = 0;
				int start, end;

				if (count < 0)
				{
					if ((index >= character_ct) && (-count >= character_ct))
						return operator=(other);

					if (index >= character_ct)
					{
						start = character_ct + count;
						end = character_ct;
					}
					else
					{
						end = index + 1;

						if (-count >= character_ct)
							start = 0;
						else
							start = end + count;
					}
				}
				else
				{
					if (index >= character_ct)
						return operator+=(other);

					if (!index && (count >= character_ct))
						return operator=(other);

					start = index;
					if (count >= character_ct)
						end = character_ct;
					else
						end = start + count;
				}


				int offset = end - start;
				int newCharCt = character_ct - offset + other.character_ct;
				this->increase(newCharCt + 1);

				if (newCharCt < character_ct)
				{
					//pull chars in
					int toOffs = newCharCt - character_ct;

					for (int i=end; i<character_ct; i++)
						data[i+toOffs] = data[i];
				}
				else if (newCharCt > character_ct)
				{
					//pull chars out
					int toPos = newCharCt + 1;
					int fromPos = character_ct + 1;

					for (int i=end; i<character_ct; i++)
						data[toPos-i] = data[fromPos-i];
				}
				//else just directly replace chars

				for (int i=0; i<other.character_ct; i++)
					data[i+start] = other.data[i];

				character_ct = newCharCt;
				data[character_ct] = 0;
			}

			return *this;
		}

		template <>
		void string<utf8>::initInt(long long value, int base, int padSize) noexcept
		{
			uint8_t ibuf[Z_STR_INT_BUFSIZE];
			if ((base < 2) || (base > 36)) base = 10;

			bool negative = false;
			if (value < 0)
			{
				value = -value;
				negative = true;
			}

			int ibufsiz = integralBuf(value, base, ibuf);

			//initialize string data
			character_ct = ibufsiz + negative;
			if (character_ct < padSize)
				character_ct += (padSize -= character_ct);
			else
				padSize = 0;

			data_len = (character_ct + 1) * this->charSize();
			data = new uint8_t[data_len];

			if (negative) this->initChar('-', 0);

			int pos = negative;

			for (int i=0; i<padSize; i++)
				this->initChar('0',pos++);

			for (int i=0; i<ibufsiz; i++)
				this->initChar(ibuf[ibufsiz-i-1], pos++);

			initChar(0, character_ct);
		}

		template <>
		void string<utf8>::initPointer(void* pointer) noexcept
		{
			uint8_t pbuf[Z_STR_INT_BUFSIZE];

			union ptv
			{
				void* pval;
				unsigned long ival;
			};
			ptv ptr;
			ptr.pval = pointer;

			int pbufsiz = integralBuf(ptr.ival, 16, pbuf);
			int padSize;

			//initialize string data
			if (Z_STR_POINTER_FORCE && (pbufsiz < Z_STR_POINTER_CHARS))
				padSize = Z_STR_POINTER_CHARS - pbufsiz;
			else
				padSize = 0;

			character_ct = padSize + pbufsiz + 2;
			data_len = (character_ct + 1) * this->charSize();
			data = new uint8_t[data_len];

			// if (negative) this->initChar('-', 0);

			int pos = 0;

			this->initChar('0',pos++);
			this->initChar('x',pos++);

			for (int i=0; i<padSize; i++)
				this->initChar('0',pos++);

			for (int i=0; i<pbufsiz; i++)
				this->initChar(pbuf[pbufsiz-i-1], pos++);
		}

		template <>
		void string<utf8>::initFloat(double value, int base, int precision, bool scientific, int padSize) noexcept
		{
			uint8_t ibuf[Z_STR_INT_BUFSIZE];
			uint8_t fbuf[Z_STR_FLOAT_BUFSIZE];
			uint8_t ebuf[Z_STR_EXP_BUFSIZE];

			data = 0;

			bool force = true;

			if ((base < 2) || (base > 36)) base = 10;
			if (precision <= 0)
			{
				precision = Z_STR_FLOAT_PRECISION;
				force = false;
			}

			bool negative = (value < 0.0);
			if (negative) value = -value;

			int exponent = 0;
			bool negexponent = false;

			//handle very small or very large numbers
			if (value && scientific)
			{
				double minVal = 1.0;
				double maxVal = 1.0;
				for (int i=0; i<precision; i++)
				{
					minVal /= base;
					maxVal *= base << 1;
				}
				minVal *= (double)base / 2.0;

				if (value < minVal)
				{
					negexponent = true;
					do
					{
						value *= base;
						++exponent;
					}
					while (value < 1.0);
				}
				else if (value > maxVal)
				{
					do
					{
						value /= base;
						++exponent;
					}
					while (value >= base);
				}
			}

			//split up parts of the number
			double intTemp = 0.0;
			double fractional = modf(value, &intTemp);
			unsigned long integral = intTemp;

			bool overflow = false;
			int ibufsiz = integralBuf(integral, base, ibuf);
			int fbufsiz = fractionalBuf(fractional, base, precision, force, fbuf, &overflow);
			if (overflow) trimFloatBuf(base, force, fbuf, &fbufsiz, ibuf, &ibufsiz);
			int ebufsiz = exponent ? integralBuf(exponent, base, ebuf) : 0;
			//initialize string data
			character_ct = ibufsiz + negative + (bool)fractional + fbufsiz + (bool)exponent + negexponent + ebufsiz;
			if (character_ct < padSize)
				character_ct += (padSize -= character_ct);
			else
				padSize = 0;


			data_len = (character_ct + 1) * this->charSize();
			data = new uint8_t[data_len];
			if (negative) this->initChar('-', 0);

			int pos = negative;

			for (int i=0; i<padSize; i++)
				this->initChar('0',pos++);

			for (int i=0; i<ibufsiz; i++)
				this->initChar(ibuf[ibufsiz-i-1], pos++);

			if (fbufsiz)
			{
				this->initChar('.', pos++);

				for (int i=0; i<fbufsiz; i++)
					this->initChar(fbuf[i],pos++);
			}

			if (exponent)
			{
				this->initChar('e', pos++);
				if (negexponent)
					this->initChar('-', pos++);

				for (int i=0; i<ebufsiz; i++)
					this->initChar(ebuf[ebufsiz-i-1],pos++);
			}

			this->initChar(0,pos);
		}

		template <>
		void string<utf8>::initComplex(const std::complex<double>& value, int base, int precision, bool scientific, int padSize) noexcept
		{
			data_len = 4;
			data = new uint8_t[data_len];
			*((uint32_t*)data) = 0;
			character_ct = 0;

			if (value.real() && value.imag())
			{
				operator=(string<utf8>(value.real(), base, precision, scientific, padSize));
				if (value.imag() > 0) operator+=("+");
				operator+=(string<utf8>(value.imag(), base, precision, scientific, padSize));
				operator+=("i");
			}
			else if (value.imag())
			{
				operator=(string<utf8>(value.imag(), base, precision, scientific, padSize));
				operator+=("i");
			}
			else
			{
				operator=(string<utf8>(value.real(), base, precision, scientific, padSize));
			}

		}

		template <>
		size_t string<utf8>::size() const noexcept
		{
			return sizeof(*this) + (data_len * sizeof(uint8_t));
		}

		template <>
		int string<utf8>::length() const noexcept
		{
			return character_ct;
		}

		template <>
		string<utf8> string<utf8>::substr(int index, int count) const noexcept
		{
			return string<utf8>(string<utf32>(*this).substr(index,count));
		}

		template <>
		long string<utf8>::integer(int base, uint32_t decimal) const noexcept
		{
			if ((base < 2) || (base > 36)) return 0;

			bool negative = (data[0] == '-');
			long result = 0;

			int start = (negative || (data[0] == '+'));

			for (int i=start; i<character_ct; i++)
			{
				uint32_t chr = data[i];

				if (isNumeric(chr))
				{
					result *= base;
					result += numeralValue(chr);
				}
				else if (chr == decimal) break;
				else return 0;
			}

			return (negative ? -result : result);
		}

		template <>
		double string<utf8>::floating(int base, uint32_t decimal) const noexcept
		{
			if ((base < 2) || (base > 36)) return 0;

			if (!character_ct) return 0;

			bool pastDecimal, pastExponent, negexponent;
			pastDecimal = pastExponent = negexponent = false;

			bool negative = (data[0] == '-');
			int start = (negative || (data[0] == '+'));

			if (start >= character_ct) return 0;

			double result = 0;
			double frac = 1;
			int exponent = 0;

			for (int i=start; i<character_ct; i++)
			{
				if (!isNumeric(data[i], base))
				{
					if (data[i] == decimal)
					{
						if (pastDecimal || pastExponent)
							return 0;
						else
						{
							if (i >= character_ct-1) return false;
							pastDecimal = true;
						}
					}
					else if (core::toLower(data[i]) == 'e')
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
		std::complex<double> string<utf8>::complex(int base, uint32_t decimal) const noexcept
		{
			if ((base < 2) || (base > 36)) return 0;
			if (!character_ct) return 0;

			int start = ((data[0] == '-') || (data[0] == '+'));
			bool imag = false;
			bool imagEnd = core::toLower(data[character_ct-1]) == 'i';
			for (int i=start; i<character_ct; ++i)
			{
				if (core::toLower(data[i]) == 'i') imag = true;
				if (((data[i] == '-') || (data[i] == '+')) && (base < 14) && i && (core::toLower(data[i-1]) != 'e'))
				{
					if (!(imag ^ imagEnd)) return 0;
					auto sub1 = substr(0,i-imag);
					auto sub2 = substr(i,character_ct-i-imagEnd);

					if (imag)
					{
						return std::complex<double>(sub2.floating(base, decimal),sub1.floating(base, decimal));
					}
					else
					{
						return std::complex<double>(sub1.floating(base, decimal),sub2.floating(base, decimal));
					}
				}
			}

			if (imagEnd)
				return std::complex<double>(0,substr(0,character_ct-1).floating(base,decimal));
			else
				return floating(base,decimal);
		}

		template <>
		int string<utf8>::type(int base, uint32_t decimal) const noexcept
		{
			if ((base < 2) || (base > 36) || !character_ct) return zstr::string;

			bool pastDecimal, pastExponent, imag, ir;
			pastDecimal = pastExponent = imag = ir = false;

			int start = ((data[0] == '-') || (data[0] == '+'));

			if (start >= character_ct) return zstr::string;

			for (int i=start; i<character_ct; i++)
			{
				if (!isNumeric(data[i], 10))
				{
					if (data[i] == decimal)
					{
						if (pastDecimal || pastExponent)
							return zstr::string;
						else
						{
							if ((i >= character_ct-1) || (core::toLower(data[i+1]) == 'i'))
								return zstr::string;
							pastDecimal = true;
						}
					}
					else if (core::toLower(data[i]) == 'e')
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
					else if (core::toLower(data[i]) == 'i')
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
		bool string<utf8>::foundAt(const string<utf8>& other, int index) const noexcept
		{
			if (index < 0) index += character_ct;
			if ((character_ct - index) < other.character_ct || index < 0) return false;

			uint32_t* data32 = (uint32_t*)data;
			uint32_t* other32 = (uint32_t*)other.data;

			const int charSz = this->charSize();

			int i = 0;
			int idx = index * charSz;
			int end = (other.character_ct * charSz) >> 2;
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
		bool string<utf8>::foundEndAt(const string<utf8>& other, int index) const noexcept
		{
			if (index < 0) index += character_ct;
			if (index < other.character_ct) return false;
			if (index >= character_ct) return false;

			const int charSz = this->charSize();
			const int idx = (index-other.character_ct+1)*charSz;
			const int last = other.character_ct * charSz;

			uint32_t* data32 = (uint32_t*)&data[idx];
			uint32_t* other32 = (uint32_t*)other.data;

			int i = 0;
			int end = last >> 2;
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
		string<utf8>& string<utf8>::remove(const string& other, int occurrence) noexcept
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

		template<>
		string<utf8>& string<utf8>::truncate(int index) noexcept
		{
			if (index >= character_ct) return *this;
			if (index < -character_ct) index = 0;
			else if (index < 0) index += character_ct;

			data[index] = 0;
			character_ct = index;
			return *this;
		}

		template <>
		string<utf8>& string<utf8>::replace(const string<utf8>& findStr, const string<utf8>& replStr, int occurrence) noexcept
		{
			if (occurrence > 0) //replace one occurrence
			{
				int pos = this->find(findStr, occurrence);

				if (pos >= 0)
				{
					this->replace(pos, findStr.length(), replStr);
				}
			}
			else if (!occurrence) //replace all occurrences
			{
				int pos = this->find(findStr, 1);

				while (pos >= 0)
				{
					this->replace(pos, findStr.length(), replStr);
					pos = this->findAfter(findStr, pos+replStr.length(), 1);
				}
			}

			return *this;
		}

		template <>
		string<utf8>& string<utf8>::padLeftIn(const string<utf8>& other, int padSize) noexcept
		{
			if (padSize <= character_ct) return *this;

			string<utf8> padStr;

			int padChars = padSize - character_ct;

			while (padChars >= other.character_ct)
			{
				padStr += other;
				padChars -= other.character_ct;
			}

			if (padChars > 0)
				padStr += other.substr(0, padChars);

			return this->insert(padStr, 0);
		}

		template <>
		string<utf8>& string<utf8>::padRightIn(const string<utf8>& other, int padSize) noexcept
		{
			if (padSize <= character_ct) return *this;

			string<utf8> padStr;

			int padChars = padSize - character_ct;

			while (padChars >= other.character_ct)
			{
				padStr += other;
				padChars -= other.character_ct;
			}

			if (padChars > 0)
				padStr += other.substr(0, padChars);

			return operator+=(padStr);
		}

		template<>
		void string<utf8>::clear() noexcept
		{
			data[0] = 0;
			character_ct = 0;
		}

		template <>
		string<utf8>& string<utf8>::cutDuplicates(const string<utf8>& other) noexcept
		{
			int pos = this->find(other, 1);

			while (pos >= 0)
			{
				int opos = pos + other.length();
				while (this->foundAt(other, opos))
				{
					this->remove(opos, other.length());
				}
				pos = this->findAfter(other, opos, 1);
			}

			return *this;
		}

		template <>
		string<utf8>& string<utf8>::toUpper() noexcept
		{
			//note that all uppercase characters are the same number of bytes as their lowercase equivalent.
			int i=0;
			while (i < character_ct)
			{
				auto ch = core::toUpper(fromUTF8(&data[i]));
				int len = toUTF8(&data[i], ch);
				i += len;
			}

			return *this;
		}

		template <>
		string<utf8>& string<utf8>::toLower() noexcept
		{
			//note that all uppercase characters are the same number of bytes as their lowercase equivalent.
			int i=0;
			while (i < character_ct)
			{
				auto ch = core::toLower(fromUTF8(&data[i]));
				int len = toUTF8(&data[i], ch);
				i += len;
			}

			return *this;
		}

		template <>
		string<utf8>& string<utf8>::toCamel() noexcept
		{
			bool doLower = false;
			//note that all uppercase characters are the same number of bytes as their lowercase equivalent.
			int i=0;
			while (i < character_ct)
			{
				auto ch = fromUTF8(&data[i]);
				ch = (doLower ? core::toLower(ch) : core::toUpper(ch, true));
				int len = toUTF8(&data[i], ch);
				doLower = isAlphaNumeric(ch) || (ch == '_');
				i += len;
			}

			return *this;
		}

		///operators
		template <>
		string<utf8> string<utf8>::operator+(const string<utf8>& other) const noexcept
		{
			string<utf8> result (*this);
			result += other;

			return result;
		}

		template <>
		string<utf8>& string<utf8>::read(std::istream& stream, uint32_t delim) noexcept
		{
			character_ct = 0;
			data[0] = 0;

			if (stream.bad() || stream.eof()) return *this;

			uint32_t last = stream.get();

			while (!stream.eof() && (delim ? (last == delim) : isWhiteSpace(last)))
				last = stream.get();

			while (!stream.eof() && !(delim ? (last == delim) : isWhiteSpace(last)))
			{
				increase(character_ct);
				data[character_ct++] = last;
				last = stream.get();
			}

			increase(character_ct);
			data[character_ct] = 0;

			return *this;
		}

		template <>
		string<utf8>& string<utf8>::readln(std::istream& stream) noexcept
		{
			character_ct = 0;
			data[0] = 0;

			if (stream.bad() || stream.eof()) return *this;

			uint32_t last = stream.get();

			while (!stream.eof())
			{
				if ((last == '\n') || (last == '\r')) break;

				increase(character_ct);
				data[character_ct++] = last;
				last = stream.get();
			}

			increase(character_ct);
			data[character_ct] = 0;

			return *this;
		}

		template<>
		bool string<utf8>::operator==(const string<utf8>& other) const noexcept
		{
			if (character_ct != other.character_ct) return false;
			if (this == &other) return true;

			for (int i=0; i<character_ct; ++i)
			{
				if (data[i] != other.data[i]) return false;
			}
			return true;
		}

		template<>
		bool string<utf8>::operator>(const string<utf8>& other) const noexcept
		{
			int len = (character_ct > other.character_ct) ? other.character_ct : character_ct;
			if (this == &other) return false;

			for (int i=0; i<len; ++i)
			{
				if (data[i] != other.data[i]) return data[i] > other.data[i];
			}
			return character_ct > other.character_ct;
		}

		template<>
		bool string<utf8>::operator<(const string<utf8>& other) const noexcept
		{
			int len = (character_ct > other.character_ct) ? other.character_ct : character_ct;
			if (this == &other) return false;

			for (int i=0; i<len; ++i)
			{
				if (data[i] != other.data[i]) return data[i] < other.data[i];
			}
			return character_ct < other.character_ct;
		}

	} //end of core namespace
} //end of z namespace
