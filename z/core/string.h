#pragma once

#include "charFunctions.h"
#include <z/encoding.h>

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

			// void appendChar(uint32_t);
		public:
			string(); //construct as null

			//single-char constructors
			string(char);
			string(wchar_t);

			//string literal constructors
			string(const char*);
			string(const wchar_t*);

			//constructors from various string types
			string(const string<ascii>&);
			string(const string<utf8>&);
			string(const string<utf16>&);
			string(const string<utf32>&);

			//copy constructor
			string(string&&);

			~string() {delete data;}


			size_t size() const;
			size_t length() const;

			const uint8_t* cstring() const;
			const uint16_t* nstring() const;
			const uint32_t* wstring() const;

			constexpr encoding format() const {return E;}
		};

		template <encoding E>
		string<E>::string(string<E>&& other)
		{
			data = other.data;
			data_len = other.data_len;
			character_ct = other.character_ct;
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

		#include "string/ascii.h"
		#include "string/utf8.h"
		#include "string/utf16.h"
		#include "string/utf32.h"
	}
}
