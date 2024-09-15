#pragma once

namespace z
{
	namespace core
	{
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
		string<utf8>::string(const char* str, size_t len) noexcept
		{
			if (str)
			{
				character_ct = len;
				data = new uint8_t[len+1];
				data_len = len+1;

				for (size_t i=0; i<len; i++)
					data[i] = str[i];

				data[len] = '\0';
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
		string<utf8>::string(const wchar_t* str, size_t __len_ignore) noexcept
		{
			(void)__len_ignore;

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
	}
}
