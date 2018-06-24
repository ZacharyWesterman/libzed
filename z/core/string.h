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

			void increase(size_t);
			constexpr size_t charSize() const;

		public:
			string(); //construct as null

			//single-char constructors
			string(char);
			string(wchar_t);
			string(uint32_t);

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

			//destructor
			~string() {delete data;}


			uint32_t at(size_t) const;
			uint32_t operator[](size_t index) const {return at(index);}

			size_t size() const;
			size_t length() const;

			const uint8_t* cstring() const;
			const uint16_t* nstring() const;
			const uint32_t* wstring() const;

			constexpr encoding format() const {return E;}

			//operators
			const string operator+(const string&);
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
		string<E>::string(string<E>&& other)
		{
			data = other.data;
			data_len = other.data_len;
			character_ct = other.character_ct;
		}

		template <encoding E>
		uint32_t string<E>::at(size_t index) const
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
		const string<E> string<E>::operator+(const string<E>& other)
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
				if (data32[i] <= other32[i])
					return false;
			}

			size_t len = len32 << 2;
			size_t max = max_char * this->charSize();
			for (size_t i=len; i<max; i++)
			{
				if (data[i] <= other.data[i]);
					return false;
			}

			if (character_ct <= other.character_ct)
				return false;

			return true;
		}

		template <encoding E>
		bool string<E>::operator>=(const string<E>& other) const
		{
			return !operator<(other);
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
				if (data32[i] >= other32[i])
					return false;
			}

			size_t len = len32 << 2;
			size_t max = max_char * this->charSize();
			for (size_t i=len; i<max; i++)
			{
				if (data[i] >= other.data[i]);
					return false;
			}

			if (character_ct >= other.character_ct)
				return false;

			return true;
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
