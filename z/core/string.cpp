#include <climits>
#include <cstdint>
#include <cstddef>
#include <cfloat>
#include <cstring>

#include "charFunctions.hpp"
#include "string.hpp"

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

#ifndef Z_STR_POINTER_FORCE
	#define Z_STR_POINTER_FORCE true
#endif

#ifndef Z_STR_POINTER_CHARS
	#define Z_STR_POINTER_CHARS 8
#endif

#define DBL_EXP_DIG ((sizeof(double) << 3) - DBL_MANT_DIG - 1)

int integralBuf(unsigned long integral, int base, uint8_t* buf) noexcept
{
	if (integral)
	{
		int length = 0;

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

int fractionalBuf(double fractional, int base, int precision, bool force, uint8_t* buf, bool* overflow) noexcept
{
	if (fractional)
	{
		fractional -= (double)(long)fractional;
		// return 0;
		int length = 0;
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

		if ((fractional * mult) >= (base / 2)) *overflow = true;

		return length;
	}
	else
	{
		return 0;
	}
}

void trimFloatBuf(int base, bool force, uint8_t* fbuf, int* fbufsiz, uint8_t* ibuf, int* ibufsiz) noexcept
{
	bool overflow = true;
	for (int i=(*fbufsiz-1); i>=0; --i)
	{
		int val = z::core::numeralValue(fbuf[i]) + 1;

		if (val >= base)
		{
			fbuf[i] = '0';
			if (!force) --(*fbufsiz);
		}
		else
		{
			fbuf[i] = z::core::numeral(val);
		}

		overflow = val > (base / 2);
		if (!overflow) break;
	}

	if (!overflow) return;

	//if fractional rounding spilled over into integer part
	for (int i=0; i<*ibufsiz; ++i)
	{
		int val = z::core::numeralValue(ibuf[i]) + 1;
		if (val >= base)
		{
			ibuf[i] = '0';
		}
		else
		{
			ibuf[i] = z::core::numeral(val);
			return;
		}
	}

	//if we need to add another digit to integer part
	//rare but it happens
	ibuf[(*ibufsiz)++] = '1';
}

namespace z
{
	namespace core
	{
		//BEGIN ASCII CONSTRUCTORS

		template <>
		string<ascii>::string() noexcept
		{
			data = new uint8_t[1];
			data[0] = 0;

			data_len = 1;
			character_ct = 0;
		}

		template <>
		string<ascii>::string(char chr) noexcept
		{
			data = new uint8_t[2];
			data[0] = chr;
			data[1] = 0;

			data_len = 2;
			character_ct = 1;
		}

		template <>
		string<ascii>::string(wchar_t chr) noexcept
		{
			data = new uint8_t[2];
			data[0] = (chr > 0xFF) ? '?' : chr;
			data[1] = 0;

			data_len = 2;
			character_ct = 1;
		}

		template <>
		string<ascii>::string(const uint32_t& chr) noexcept
		{
			data = new uint8_t[2];
			data[0] = (chr > 0xFF) ? '?' : chr;
			data[1] = 0;

			data_len = 2;
			character_ct = 1;
		}

		template <>
		string<ascii>::string(const char* str) noexcept
		{
			if (str)
			{
				int len = 0;
				while (str[len]) len++;

				character_ct = len;
				data = new uint8_t[++len];
				data_len = len;

				for (int i=0; i<len; i++)
					data[i] = str[i];
			}
			else
			{
				data = new uint8_t[1];
				data[0] = 0;

				data_len = 1;
				character_ct = 0;
			}
		}

		template <>
		string<ascii>::string(const wchar_t* str) noexcept
		{
			if (str)
			{
				int len = 0;
				while (str[len]) len++;

				character_ct = len;
				data = new uint8_t[++len];
				data_len = len;

				for (int i=0; i<len; i++)
				{
					if (str[i] > 0xFF)
						data[i] = '?';
					else
						data[i] = str[i];
				}
			}
			else
			{
				data = new uint8_t[1];
				data[0] = 0;

				data_len = 1;
				character_ct = 0;
			}
		}

		template <>
		string<ascii>::string(const string<ascii>& other) noexcept
		{
			data_len = other.character_ct + 1;
			character_ct = other.character_ct;

			data = new uint8_t[data_len];

			for (int i=0; i<data_len; i++)
				data[i] = other.data[i];
		}

		template <>
		string<ascii>::string(const string<utf8>& other) noexcept
		{
			data_len = other.character_ct + 1;
			character_ct = other.character_ct;

			data = new uint8_t[data_len];
			data[character_ct] = 0;

			int pos = 0;
			for (int i=0; i<data_len; i++)
			{
				uint32_t val = fromUTF8(&other.data[pos]);
				pos += lenFromUTF8(&other.data[pos]);

				data[i] = (val > 0xFF) ? '?' : val;
			}
		}

		template <>
		string<ascii>::string(const string<utf16>& other) noexcept
		{
			data_len = other.character_ct + 1;
			character_ct = other.character_ct;

			data = new uint8_t[data_len];
			data[character_ct] = 0;

			uint16_t* data16 = (uint16_t*)other.data;

			for (int i=0; i<data_len; i++)
			{
				data[i] = (data16[i] > 0xFF) ? '?' : data16[i];
			}
		}

		template <>
		string<ascii>::string(const string<utf32>& other) noexcept
		{
			data_len = other.character_ct + 1;
			character_ct = other.character_ct;

			data = new uint8_t[data_len];
			data[character_ct] = 0;

			uint32_t* data32 = (uint32_t*)other.data;

			for (int i=0; i<= character_ct; i++)
				data[i] = (data32[i] > 0xFF) ? '?' : (uint8_t)data32[i];
		}

		//BEGIN UTF-8 CONSTRUCTORS

		template <>
		string<utf8>::string() noexcept
		{
			data = new uint8_t[1];
			data[0] = 0;

			data_len = 1;
			character_ct = 0;
		}

		template <>
		string<utf8>::string(char chr) noexcept
		{
			data = new uint8_t[2];
			data[0] = chr;
			data[1] = 0;

			data_len = 2;
			character_ct = 1;
		}

		template <>
		string<utf8>::string(wchar_t chr) noexcept
		{
			data = new uint8_t[5];
			data_len = 5;

			int len = toUTF8(data, chr);
			data[len] = 0;

			character_ct = 1;
		}

		template <>
		string<utf8>::string(const uint32_t& chr) noexcept
		{
			data = new uint8_t[5];
			data_len = 5;

			int len = toUTF8(data, chr);
			data[len] = 0;

			character_ct = 1;
		}

		template <>
		string<utf8>::string(const char* str) noexcept
		{
			if (str)
			{
				int len = 0;
				while (str[len]) len++;

				character_ct = len;
				data = new uint8_t[++len];
				data_len = len;

				for (int i=0; i<len; i++)
					data[i] = str[i];
			}
			else
			{
				data = new uint8_t[1];
				data[0] = 0;

				data_len = 1;
				character_ct = 0;
			}
		}

		template <>
		string<utf8>::string(const wchar_t* str) noexcept
		{
			if (str)
			{
				int i = 0;
				int len = 0;

				while (str[i])
					len += lenToUTF8(str[i++]);

				character_ct = len;

				data_len = character_ct + 1;
				data = new uint8_t[data_len];
				data[character_ct] = 0;

				int pos = 0;
				for (int j=0; j<i; j++)
				{
					pos += toUTF8(&data[pos], str[j]);
				}
			}
			else
			{
				data = new uint8_t[1];
				data[0] = 0;

				data_len = 1;
				character_ct = 0;
			}
		}

		template <>
		string<utf8>::string(const string<ascii>& other) noexcept
		{
			data_len = 1;
			character_ct = other.character_ct;

			for (int i=0; i<other.data_len; i++)
				data_len += lenToUTF8(other.data[i]);

			data = new uint8_t[data_len];
			data[data_len-1] = 0;

			int len = 0;
			for (int i=0; i<character_ct; i++)
				len += toUTF8(&data[len], other.data[i]);

			character_ct = data_len - 1;
		}

		template <>
		string<utf8>::string(const string<utf8>& other) noexcept
		{
			data_len = other.data_len;
			character_ct = other.character_ct;

			data = new uint8_t[data_len];

			for (int i=0; i<data_len; i++)
				data[i] = other.data[i];
		}

		template <>
		string<utf8>::string(const string<utf16>& other) noexcept
		{
			data_len = 1;
			character_ct = other.character_ct;

			uint16_t* data16 = (uint16_t*)other.data;

			for (int i=0; i<=other.character_ct; i++)
				data_len += lenToUTF8(data16[i]);

			data = new uint8_t[data_len];
			data[data_len-1] = 0;

			int len = 0;
			for (int i=0; i<character_ct; i++)
				len += toUTF8(&data[len], data16[i]);

			character_ct = len;
		}

		template <>
		string<utf8>::string(const string<utf32>& other) noexcept
		{
			data_len = 1;
			character_ct = other.character_ct;

			uint32_t* data32 = (uint32_t*)other.data;

			for (int i=0; i<other.character_ct; i++)
				data_len += lenToUTF8(data32[i]);

			data = new uint8_t[data_len];
			data[data_len-1] = 0;

			int len = 0;
			for (int i=0; i<character_ct; i++)
				len += toUTF8(&data[len], data32[i]);

			character_ct = len;
		}

		//BEGIN UTF-16 CONSTRUCTORS

		template <>
		string<utf16>::string() noexcept
		{
			data = new uint8_t[2];
			*((uint16_t*)data) = 0;

			data_len = 2;
			character_ct = 0;
		}

		template <>
		string<utf16>::string(char chr) noexcept
		{
			data = new uint8_t[4];
			uint16_t* data16 = (uint16_t*)data;

			data16[0] = chr;
			data16[1] = 0;

			data_len = 4;
			character_ct = 1;
		}

		template <>
		string<utf16>::string(wchar_t chr) noexcept
		{
			data = new uint8_t[4];
			uint16_t* data16 = (uint16_t*)data;

#			ifdef _WIN32
			data16[0] = chr; //In Windows, wchar_t is 16 bits
#			else
			data16[0] = (chr > 0xFFFF) ? '?' : chr;
#			endif
			data16[1] = 0;

			data_len = 4;
			character_ct = 1;
		}

		template <>
		string<utf16>::string(const uint32_t& chr) noexcept
		{
			data = new uint8_t[4];
			uint16_t* data16 = (uint16_t*)data;

			data16[0] = (chr > 0xFFFF) ? '?' : chr;
			data16[1] = 0;

			data_len = 4;
			character_ct = 1;
		}

		template <>
		string<utf16>::string(const char* str) noexcept
		{
			if (str)
			{
				int len = 0;
				while (str[len]) len++;

				character_ct = len;
				data = new uint8_t[(++len)<<1];
				data_len = len<<1;

				uint16_t* data16 = (uint16_t*)data;

				for (int i=0; i<len; i++)
					data16[i] = str[i];
			}
			else
			{
				data = new uint8_t[2];
				*((uint16_t*)data) = 0;

				data_len = 2;
				character_ct = 0;
			}
		}

		template <>
		string<utf16>::string(const wchar_t* str) noexcept
		{
			if (str)
			{
				int len = 0;
				while (str[len]) len++;

				character_ct = len;
				data = new uint8_t[(++len)<<1];
				data_len = len<<1;

				uint16_t* data16 = (uint16_t*)data;

				for (int i=0; i<len; i++)
				{
#					ifdef _WIN32
					data16[i] = str[i]; //In Windows, wchar_t is 16 bits
#					else
					data16[i] = (str[i] > 0xFFFF) ? '?' : str[i];
#					endif
				}
			}
			else
			{
				data = new uint8_t[2];
				*((uint16_t*)data) = 0;

				data_len = 2;
				character_ct = 0;
			}
		}

		template <>
		string<utf16>::string(const string<ascii>& other) noexcept
		{
			data_len = (other.character_ct + 1) << 1;
			character_ct = other.character_ct;

			data = new uint8_t[data_len];
			uint16_t* data16 = (uint16_t*)data;

			for (int i=0; i<=character_ct; i++)
				data16[i] = other.data[i];
		}

		template <>
		string<utf16>::string(const string<utf8>& other) noexcept
		{
			character_ct = 0;
			for (int i=0; i<other.character_ct; i+=lenFromUTF8(&other.data[i]))
				character_ct++;

			data_len = (character_ct + 1) << 1;
			data = new uint8_t[data_len];

			uint16_t* data16 = (uint16_t*)data;
			data16[character_ct] = 0;

			int pos = 0;
			for (int i=0; i<=character_ct; i++)
			{
				uint32_t val = fromUTF8(&other.data[pos]);
				pos += lenFromUTF8(&other.data[pos]);

				data16[i] = (val > 0xFFFF) ? '?' : val;
			}
		}

		template <>
		string<utf16>::string(const string<utf16>& other) noexcept
		{
			data_len = (other.character_ct + 1) << 1;
			character_ct = other.character_ct;

			data = new uint8_t[data_len];

			uint16_t* data16 = (uint16_t*)data;
			uint16_t* other16 = (uint16_t*)other.data;

			for (int i=0; i<=character_ct; i++)
				data16[i] = other16[i];
		}

		template <>
		string<utf16>::string(const string<utf32>& other) noexcept
		{
			data_len = (other.character_ct + 1) << 1;
			character_ct = other.character_ct;

			data = new uint8_t[data_len];

			uint16_t* data16 = (uint16_t*)data;
			uint32_t* data32 = (uint32_t*)other.data;

			for (int i=0; i<=character_ct; i++)
				data16[i] = (data32[i] > 0xFFFF) ? '?' : data32[i];
		}

		//BEGIN UTF-32 CONSTRUCTORS

		template <>
		string<utf32>::string() noexcept
		{
			data = new uint8_t[4];
			*((uint32_t*)data) = 0;

			data_len = 4;
			character_ct = 0;
		}

		template <>
		string<utf32>::string(char chr) noexcept
		{
			data = new uint8_t[4];
			uint32_t* data32 = (uint32_t*)data;

			data32[0] = chr;
			data32[1] = 0;

			data_len = 8;
			character_ct = 1;
		}

		template <>
		string<utf32>::string(wchar_t chr) noexcept
		{
			data = new uint8_t[4];
			uint32_t* data32 = (uint32_t*)data;

			data32[0] = chr;
			data32[1] = 0;

			data_len = 8;
			character_ct = 1;
		}

		template <>
		string<utf32>::string(const uint32_t& chr) noexcept
		{
			data = new uint8_t[4];
			uint32_t* data32 = (uint32_t*)data;

			data32[0] = chr;
			data32[1] = 0;

			data_len = 8;
			character_ct = 1;
		}

		template <>
		string<utf32>::string(const char* str) noexcept
		{
			if (str)
			{
				int len = 0;
				while (str[len]) len++;

				character_ct = len;
				data = new uint8_t[(++len)<<2];
				data_len = len<<2;

				uint32_t* data32 = (uint32_t*)data;

				for (int i=0; i<len; i++)
					data32[i] = str[i];
			}
			else
			{
				data = new uint8_t[4];
				*((uint32_t*)data) = 0;

				data_len = 4;
				character_ct = 0;
			}
		}

		template <>
		string<utf32>::string(const wchar_t* str) noexcept
		{
			if (str)
			{
				int len = 0;
				while (str[len]) len++;

				character_ct = len;
				data = new uint8_t[(++len)<<2];
				data_len = len<<2;

				uint32_t* data32 = (uint32_t*)data;

				for (int i=0; i<len; i++)
					data32[i] = str[i];
			}
			else
			{
				data = new uint8_t[4];
				*((uint32_t*)data) = 0;

				data_len = 4;
				character_ct = 0;
			}
		}

		template <>
		string<utf32>::string(const string<ascii>& other) noexcept
		{
			data_len = (other.character_ct + 1) << 2;
			character_ct = other.character_ct;

			data = new uint8_t[data_len];
			uint32_t* data32 = (uint32_t*)data;

			for (int i=0; i<=character_ct; i++)
				data32[i] = other.data[i];
		}

		template <>
		string<utf32>::string(const string<utf8>& other) noexcept
		{
			character_ct = 0;
			for (int i=0; i<other.character_ct; i+=lenFromUTF8(&other.data[i]))
				character_ct++;

			data_len = (character_ct + 1) << 2;
			data = new uint8_t[data_len];

			uint32_t* data32 = (uint32_t*)data;
			data32[character_ct] = 0;

			int pos = 0;
			for (int i=0; i<character_ct; i++)
			{
				uint32_t val = fromUTF8(&other.data[pos]);
				pos += lenFromUTF8(&other.data[pos]);

				data32[i] = val;
			}
		}

		template <>
		string<utf32>::string(const string<utf16>& other) noexcept
		{
			data_len = (other.character_ct + 1) << 2;
			character_ct = other.character_ct;

			data = new uint8_t[data_len];

			uint32_t* data32 = (uint32_t*)data;
			uint16_t* data16 = (uint16_t*)other.data;

			for (int i=0; i<=character_ct; i++)
				data32[i] = data16[i];
		}

		template <>
		string<utf32>::string(const string<utf32>& other) noexcept
		{
			data_len = (other.character_ct + 1) << 2;
			character_ct = other.character_ct;

			data = new uint8_t[data_len];

			uint32_t* data32 = (uint32_t*)data;
			uint32_t* other32 = (uint32_t*)other.data;

			for (int i=0; i<=character_ct; i++)
				data32[i] = other32[i];
		}

		//END OF CONSTRUCTORS
		//BEGIN VARIOUS PRIVATE ENCODING-UNIQUE FUNCTIONS

		template<>
		uint32_t string<ascii>::getChar(int index) const noexcept
		{
			return data[index];
		}

		template<>
		void string<ascii>::setChar(uint32_t value, int index) noexcept
		{
			data[index] = value;
		}

		template<>
		int string<ascii>::charPower() const noexcept
		{
			return 0;
		}

		template<>
		void string<ascii>::increase(int charCount) noexcept
		{
			int goal_len = charCount + 1;
			if (goal_len > data_len)
			{
				auto new_data = new uint8_t[goal_len];
				memcpy(new_data, data, data_len);
			}
		}

		template<>
		uint32_t string<utf8>::getChar(int index) const noexcept
		{
			return fromUTF8(&data[index]);
		}

		template<>
		void string<utf8>::setChar(uint32_t value, int index) noexcept
		{
			int len = toUTF8(&data[index], value);
		}

		template<>
		int string<utf8>::charPower() const noexcept
		{
			return 0;
		}

		template<>
		void string<utf8>::increase(int charCount) noexcept
		{
			int goal_len = charCount + 1;
			if (goal_len > data_len)
			{
				auto new_data = new uint8_t[goal_len];
				memcpy(new_data, data, data_len);
			}
		}

		template<>
		uint32_t string<utf16>::getChar(int index) const noexcept
		{
			auto data16 = (uint16_t*)data;
			return data16[index];
		}

		template<>
		void string<utf16>::setChar(uint32_t value, int index) noexcept
		{
			auto data16 = (uint16_t*)data;
			data16[index] = value;
		}

		template<>
		int string<utf16>::charPower() const noexcept
		{
			return 1;
		}

		template<>
		void string<utf16>::increase(int charCount) noexcept
		{
			int goal_len = (charCount + 1) << 1;
			if (goal_len > data_len)
			{
				auto new_data = new uint8_t[goal_len];
				memcpy(new_data, data, data_len);
			}
		}

		template<>
		uint32_t string<utf32>::getChar(int index) const noexcept
		{
			auto data32 = (uint32_t*)data;
			return data32[index];
		}

		template<>
		void string<utf32>::setChar(uint32_t value, int index) noexcept
		{
			auto data32 = (uint32_t*)data;
			data32[index] = value;
		}

		template<>
		int string<utf32>::charPower() const noexcept
		{
			return 2;
		}

		template<>
		void string<utf32>::increase(int charCount) noexcept
		{
			int goal_len = (charCount + 1) << 1;
			if (goal_len > data_len)
			{
				auto new_data = new uint8_t[goal_len];
				memcpy(new_data, data, data_len);
			}
		}

		//END OF VARIOUS PRIVATE ENCODING-UNIQUE FUNCTIONS
		//BEGIN PUBLIC ENCODING-UNIQUE FUNCTIONS


	}
}


std::ostream& operator<<(std::ostream& stream, const z::core::string<z::utf8>& object)
{
	return stream << object.cstring();
}

//Custom literals for simple string construction
z::core::string<z::utf32> operator "" _u32(char value) { return z::core::string<z::utf32>(value); }
z::core::string<z::utf32> operator "" _u32(wchar_t value) { return z::core::string<z::utf32>(value); }
z::core::string<z::utf32> operator "" _u32(const char* value) { return z::core::string<z::utf32>(value); }
z::core::string<z::utf32> operator "" _u32(const char* value, size_t) { return z::core::string<z::utf32>(value); }
z::core::string<z::utf32> operator "" _u32(const wchar_t* value, size_t) { return z::core::string<z::utf32>(value); }

z::core::string<z::utf16> operator "" _u16(char value) { return z::core::string<z::utf16>(value); }
z::core::string<z::utf16> operator "" _u16(wchar_t value) { return z::core::string<z::utf16>(value); }
z::core::string<z::utf16> operator "" _u16(const char* value) { return z::core::string<z::utf16>(value); }
z::core::string<z::utf16> operator "" _u16(const char* value, size_t) { return z::core::string<z::utf16>(value); }
z::core::string<z::utf16> operator "" _u16(const wchar_t* value, size_t) { return z::core::string<z::utf16>(value); }

z::core::string<z::utf8> operator "" _u8(char value) { return z::core::string<z::utf8>(value); }
z::core::string<z::utf8> operator "" _u8(wchar_t value) { return z::core::string<z::utf8>(value); }
z::core::string<z::utf8> operator "" _u8(const char* value) { return z::core::string<z::utf8>(value); }
z::core::string<z::utf8> operator "" _u8(const char* value, size_t) { return z::core::string<z::utf8>(value); }
z::core::string<z::utf8> operator "" _u8(const wchar_t* value, size_t) { return z::core::string<z::utf8>(value); }

z::core::string<z::ascii> operator "" _asc(char value) { return z::core::string<z::ascii>(value); }
z::core::string<z::ascii> operator "" _asc(wchar_t value) { return z::core::string<z::ascii>(value); }
z::core::string<z::ascii> operator "" _asc(const char* value) { return z::core::string<z::ascii>(value); }
z::core::string<z::ascii> operator "" _asc(const char* value, size_t) { return z::core::string<z::ascii>(value); }
z::core::string<z::ascii> operator "" _asc(const wchar_t* value, size_t) { return z::core::string<z::ascii>(value); }
