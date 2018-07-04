#pragma once

#include <type_traits>
#include <complex>

#include "charFunctions.h"
#include <z/encoding.h>

#ifndef Z_STR_INT_BUFSIZE
	#define Z_STR_INT_BUFSIZE 64
#endif

#ifndef Z_STR_FLOAT_BUFSIZE
	#define Z_STR_FLOAT_BUFSIZE 64
#endif

#ifndef Z_STR_EXP_BUFSIZE
	#define Z_STR_EXP_BUFSIZE 10
#endif

#ifndef Z_STR_FLOAT_PRECISION
	#define Z_STR_FLOAT_PRECISION 6
#endif

#ifndef Z_STR_EXP_SCIENTIFIC
	#define Z_STR_EXP_SCIENTIFIC 50
#endif

#ifndef Z_STR_FLOAT_ROUND
	#define Z_STR_FLOAT_ROUND 0.0000005
#endif

#ifndef Z_STR_POINTER_FORCE
	#define Z_STR_POINTER_FORCE true
#endif

#ifndef Z_STR_POINTER_CHARS
	#define Z_STR_POINTER_CHARS 8
#endif


static size_t integralBuf(unsigned long integral, int base, uint8_t* buf)
{
	if (integral)
	{
		size_t length = 0;

		while (integral)
		{
			buf[length] = z::core::numeral(integral%base);
			integral /= base;
			length++;
		}

		return length;
	}
	else
	{
		buf[0] = '0';
		return 1;
	}
}

static size_t fractionalBuf(double fractional, int base, int precision, bool force, uint8_t* buf)
{
	if (fractional)
	{
		fractional -= (double)(long)fractional;
		// return 0;
		size_t length = 0;
		double mult = base;
		int i = 0;

		bool cont = true;
		while ((i < precision) && cont)
		{
			fractional *= mult;

			uint8_t chr = z::core::numeral((int)fractional);
			fractional -= (double)(long)fractional;

			buf[i] = chr;

			i++;
			if (chr != '0') length = i;
			if (!fractional && !force) cont = false;
		}

		return length;
	}
	else
	{
		return 0;
	}
}

namespace z
{
	namespace core
	{
		template <encoding E>
		class string
		{
			friend string<ascii>;
			friend string<utf8>;
			friend string<utf16>;
			friend string<utf32>;

		private:
			uint8_t* data;
			size_t data_len;
			size_t character_ct;

			void initChar(uint32_t, size_t);
			void increase(size_t); //increase number of string bytes up to the given amount
			constexpr size_t charSize() const;

		public:
			string(); //construct as null

			//single-char constructors
			string(char);
			string(wchar_t);
			string(const uint32_t&);

			//string literal constructors
			string(const char*);
			string(const wchar_t*);

			//constructors from numerical types
			template <typename INT, typename = typename std::enable_if<std::is_integral<INT>::value,INT>::type>
			string(INT, int base = 10, int padSize = 0);

			template <typename PTR, typename = typename std::enable_if<std::is_pointer<PTR>::value,PTR>::type>
			string(PTR);

			string(double, int base = 10, int precision = 0, int padSize = 0);
			string(const std::complex<double>&, int base = 10, int precision = 0);


			//constructors from various string types
			explicit string(const string<ascii>&);
			explicit string(const string<utf8>&);
			explicit string(const string<utf16>&);
			explicit string(const string<utf32>&);

			//copy constructor
			string(string&&);

			//destructor
			~string() {delete[] data;}


			const uint32_t at(size_t) const;
			const uint32_t operator[](size_t index) const {return at(index);}

			size_t size() const;
			size_t length() const;
			size_t chars() const;

			const uint8_t* cstring() const;
			const uint16_t* nstring() const;
			const uint32_t* wstring() const;

			constexpr encoding format() const {return E;}

			long integer(int base = 10) const;
			double floating(int base = 10) const;
			std::complex<double> complex(int base = 10) const;

			///analyzers
			int count(const string&) const;

			int find(const string&, int occurrence = 1) const;
			int findLast(const string&, int occurrence = 1) const;
			int findAfter(const string&, size_t, int occurrence = 1) const;
			int findBefore(const string&, size_t, int occurrence = 1) const;

			bool foundAt(const string&, size_t) const;
			bool foundEndAt(const string&, size_t) const;

			bool beginsWith(const string&) const;
			bool endsWith(const string&) const;

			bool isInteger(int base = 10) const;
			bool isFloating(int base = 10) const;
			bool isComplex(int base = 10) const;

			///mutators
			string substr(size_t, int) const;

			const string& append(const string& other) {return operator+=(other);}
			const string& insert(const string&, size_t); //insert before index
			const string& remove(const string&, int occurrence = 0);
			const string& remove(size_t, int);

			const string& replace(const string&, const string&, int occurrence = 0);
			const string& replace(size_t, int, const string&);

			const string& padLeft(const string&, size_t);
			const string& padRight(const string&, size_t);

			const string& unPadLeft(const string&);
			const string& unPadRight(const string&);
			const string& unPad(const string&);

			const string& cutDuplicates(const string&);


			//operators
			string operator+(const string&);
			const string& operator+=(const string&);

			const string& operator=(const string&);

			bool operator==(const string&) const;
			bool operator!=(const string&) const;
			bool operator>(const string&) const;
			bool operator>=(const string&) const;
			bool operator<(const string&) const;
			bool operator<=(const string&) const;
		};

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
				if ((1023 + Z_STR_EXP_SCIENTIFIC) <= number.exponent)// pos exponent <= x2^50
				{
					while (number.fval >= base)
					{
						number.fval /= base;
						exponent++;
					}
				}
				else if ((1023 - Z_STR_EXP_SCIENTIFIC) >= number.exponent)// neg exponent >= x2^-50
				{
					negexponent = true;

					while (number.fval < 1)
					{
						number.fval *= base;
						exponent++;
					}
				}
			}

			if (number.exponent < 1023)//x2^neg
			{
				integral = 0;
			}
			else if (number.exponent > 1023)//x2^(pos)
			{
				uint32_t expo = number.exponent - 1023;
				integral = (1 << expo) + (number.mantissa >> (52 - expo));
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

			for (size_t i=start; i<character_ct; i++)
			{
				if (!isNumeric(data[i], base))
				{
					if (data[i] == '.')
					{
						if (pastDecimal || pastExponent)
							return false;
						else
							pastDecimal = true;
					}
					else if (toLower(data[i]) == 'e')
					{
						if (pastExponent)
							return false;
						else
							pastExponent = true;
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
		string<E> string<E>::operator+(const string<E>& other)
		{
			string<E> result = *this;
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


		#include "string/utf32.h"
		#include "string/utf16.h"
		#include "string/ascii.h"
		#include "string/utf8.h"
	}
}
