#pragma once

namespace z
{
	namespace core
	{
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
		string<ascii>::string(const char* str, size_t len) noexcept
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
		string<ascii>::string(const wchar_t* str, size_t len) noexcept
		{
			if (str)
			{
				character_ct = len;
				data = new uint8_t[len+1];
				data_len = len+1;

				for (size_t i=0; i<len; i++)
				{
					if (str[i] > 0xFF)
						data[i] = '?';
					else
						data[i] = str[i];
				}

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
	}
}
