#pragma once

#include <type_traits>
#include <complex>
#include <climits>

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

#ifndef Z_STR_POINTER_FORCE
	#define Z_STR_POINTER_FORCE true
#endif

#ifndef Z_STR_POINTER_CHARS
	#define Z_STR_POINTER_CHARS 8
#endif


static size_t integralBuf(unsigned long integral, int base, uint8_t* buf);
static size_t fractionalBuf(double fractional, int base, int precision, bool force, uint8_t* buf);

namespace z
{
	namespace core
	{
		/**
         * \brief A template class for character strings.
         *
         * This class focuses on how characters are encoded rather than character size.
		 * Possible encoding schemes are ASCII, UTF-8, UTF16, and UTF32.
		 * When characters are input or output, they are converted to or from their
		 * encoding scheme to a single UTF32 character. Thus, the "default" character
		 * type is `uint32_t`.
         * <br/><br/>
		 * Allocated memory is increased as needed with approximate 1.5x growth, and
		 * is not decreased on subsequent data changes, except in the case where
		 * data is copied over to a different string.
         */
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
			void increase(size_t); //increase number of data bytes up to the given amount
			constexpr size_t charSize() const;

		public:
			///Default string constructor
			string();

			/**
			 * \brief Construct string from a single-byte character.
			 *
			 * \param chr Initializing character.
			 *
			 * Character is assumed to be compatible with this string's encoding.
			 */
			string(char chr);

			/**
			 * \brief Construct string from a wide character.
			 *
			 * \param chr Initializing character.
			 *
			 * Converts the given character to the appropriate encoding for this string.
			 */
			string(wchar_t chr);

			/**
			 * \brief Construct string from uint32_t.
			 *
			 * \param chr Initializing character.
			 *
			 * This constructor exists to allow a "default" string character to
			 * again be constructed into a string.
			 * Converts the given character to the appropriate encoding for this string.
			 *
			 * \see at()
			 * \see operator[]()
			 */
			string(const uint32_t& chr);

			/**
			 * \brief Construct from a cstring of single-byte characters.
			 *
			 * \param str Null-terminated cstring.
			 *
			 * All characters are assumed to be compatible with this string's encoding.
			 */
			string(const char* str);

			/**
			 * \brief Construct from a cstring of wide characters.
			 *
			 * \param str Null-terminated cstring.
			 *
			 * Converts the characters in the given cstring to the appropriate encoding
			 * for this string.
			 */
			string(const wchar_t* str);

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

			/// Lvalue copy-constructor
			string(string&&);

			//destructor
			~string();

			///at
			const uint32_t at(size_t) const;

			///operator[]
			const uint32_t operator[](size_t index) const;

			size_t size() const;
			size_t length() const;
			size_t chars() const;

			const uint8_t* cstring() const;
			const uint16_t* nstring() const;
			const uint32_t* wstring() const;

			constexpr encoding format() const;

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

		#include "string/default.h"
		#include "string/utf32.h"
		#include "string/utf16.h"
		#include "string/ascii.h"
		#include "string/utf8.h"
	}
}


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
