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

#include "string/asciiConstructors.hpp"
#include "string/utf8Constructors.hpp"
#include "string/utf16Constructors.hpp"
#include "string/utf32Constructors.hpp"

#include "string/ascii.hpp"
#include "string/utf16.hpp"
#include "string/utf32.hpp"
#include "string/utf8.hpp"

std::ostream& operator<<(std::ostream& stream, const z::core::string<z::utf8>& object)
{
	return stream << object.cstring();
}

//Custom literals for simple string construction
z::core::string<z::utf32> operator "" _u32(char value) { return z::core::string<z::utf32>(value); }
z::core::string<z::utf32> operator "" _u32(wchar_t value) { return z::core::string<z::utf32>(value); }
z::core::string<z::utf32> operator "" _u32(const char* value) { return z::core::string<z::utf32>(value); }
z::core::string<z::utf32> operator "" _u32(const char* value, size_t len) { return z::core::string<z::utf32>(value, len); }
z::core::string<z::utf32> operator "" _u32(const wchar_t* value, size_t len) { return z::core::string<z::utf32>(value, len); }

z::core::string<z::utf16> operator "" _u16(char value) { return z::core::string<z::utf16>(value); }
z::core::string<z::utf16> operator "" _u16(wchar_t value) { return z::core::string<z::utf16>(value); }
z::core::string<z::utf16> operator "" _u16(const char* value) { return z::core::string<z::utf16>(value); }
z::core::string<z::utf16> operator "" _u16(const char* value, size_t len) { return z::core::string<z::utf16>(value, len); }
z::core::string<z::utf16> operator "" _u16(const wchar_t* value, size_t len) { return z::core::string<z::utf16>(value, len); }

z::core::string<z::utf8> operator "" _u8(char value) { return z::core::string<z::utf8>(value); }
z::core::string<z::utf8> operator "" _u8(wchar_t value) { return z::core::string<z::utf8>(value); }
z::core::string<z::utf8> operator "" _u8(const char* value) { return z::core::string<z::utf8>(value); }
z::core::string<z::utf8> operator "" _u8(const char* value, size_t len) { return z::core::string<z::utf8>(value, len); }
z::core::string<z::utf8> operator "" _u8(const wchar_t* value, size_t len) { return z::core::string<z::utf8>(value, len); }

z::core::string<z::ascii> operator "" _asc(char value) { return z::core::string<z::ascii>(value); }
z::core::string<z::ascii> operator "" _asc(wchar_t value) { return z::core::string<z::ascii>(value); }
z::core::string<z::ascii> operator "" _asc(const char* value) { return z::core::string<z::ascii>(value); }
z::core::string<z::ascii> operator "" _asc(const char* value, size_t len) { return z::core::string<z::ascii>(value, len); }
z::core::string<z::ascii> operator "" _asc(const wchar_t* value, size_t len) { return z::core::string<z::ascii>(value, len); }

zstring operator "" _zs(char value) { return zstring(value); }
zstring operator "" _zs(wchar_t value) { return zstring(value); }
zstring operator "" _zs(const char* value) { return zstring(value); }
zstring operator "" _zs(const char* value, size_t len) { return zstring(value, len); }
zstring operator "" _zs(const wchar_t* value, size_t len) { return zstring(value, len); }