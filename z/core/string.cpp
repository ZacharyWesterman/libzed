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

int integralBuf(unsigned long integral, int base, uint8_t* buf)
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

int fractionalBuf(double fractional, int base, int precision, bool force, uint8_t* buf)
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

		return length;
	}
	else
	{
		return 0;
	}
}

#include "string/asciiConstructors.hpp"
#include "string/utf8Constructors.hpp"
#include "string/utf16Constructors.hpp"
#include "string/utf32Constructors.hpp"

#include "string/ascii.hpp"
#include "string/utf16.hpp"
#include "string/utf32.hpp"
#include "string/utf8.hpp"
