#pragma once

#include <type_traits>

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
		size_t length = 0;
		double mult = base;

		for (int i=0; i<precision; i++)
		{
			if (!(fractional || force)) return length;

			char chr = z::core::numeral(fractional *= mult);
			fractional -= (long)fractional;

			buf[length] = (chr);

			if (chr != '0') length = i+1;
		}

		if (force)
			return precision;
		else
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


			//constructors from various string types
			string(const string<ascii>&);
			string(const string<utf8>&);
			string(const string<utf16>&);
			string(const string<utf32>&);

			//copy constructor
			string(string&&);

			//destructor
			~string() {delete data;}


			const uint32_t at(size_t) const;
			const uint32_t operator[](size_t index) const {return at(index);}

			size_t size() const;
			size_t length() const;

			const uint8_t* cstring() const;
			const uint16_t* nstring() const;
			const uint32_t* wstring() const;

			constexpr encoding format() const {return E;}

			///analyzers
			int count(const string&);

			int find(const string&, int occurrence = 1) const;
			int findLast(const string&, int occurrence = 1) const;
			int findAfter(const string&, size_t) const;
			int findBefore(const string&, size_t) const;

			int foundAt(const string&, size_t) const;
			int foundEndAt(const string&, size_t) const;

			bool beginsWith(const string&) const;
			bool endsWith(const string&) const;

			///mutators
			string substr(size_t, int);

			const string& append(const string& other) {return operator+=(other);}
			const string& insert(const string&, size_t);
			const string& remove(const string&, int occurrence = 0);
			const string& remove(size_t, int);

			const string& replace(const string&, const string&, int occurrence = 0);
			const string& replace(size_t, int, const string&);

			const string& padLeft(const string&, size_t);
			const string& padRight(const string&, size_t);

			const string&  unPadLeft(const string&);
			const string&  unPadRight(const string&);
			const string&  cutDuplicates(const string&);


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

				while (number.exponent < 1023)
				{
					number.fval *= base;
					exponent++;
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
			// number.exponent - 1023;

			size_t ibufsiz = integralBuf(integral, base, ibuf);
			size_t fbufsiz = fractionalBuf(fractional, base, precision, force, fbuf);
			size_t ebufsiz = exponent ? integralBuf(exponent, base, ebuf) : 0;

			//initialize string data
			character_ct = ibufsiz + negative + (bool)fractional + fbufsiz + (bool)exponent + negexponent + ebufsiz;
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

			if (fractional)
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

			this->initChar(0,character_ct);
		}

		template <encoding E>
		string<E>::string(string<E>&& other)
		{
			data = other.data;
			data_len = other.data_len;
			character_ct = other.character_ct;
		}

		template <encoding E>
		const uint32_t string<E>::at(size_t index) const
		{
			if (index < character_ct)
				return data[index];
			else
				return 0;
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
			delete[] data;
			data = new uint8_t[other.data_len];
			data_len = other.data_len;
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


		#include "string/ascii.h"
		#include "string/utf8.h"
		#include "string/utf16.h"
		#include "string/utf32.h"
	}
}
