#pragma once

template <>
string<utf32>::string()
{
	data = new uint8_t[4];
	*((uint32_t*)data) = 0;

	data_len = 4;
	character_ct = 0;
}

template <>
string<utf32>::string(char chr)
{
	data = new uint8_t[4];
	uint32_t* data32 = (uint32_t*)data;

	data32[0] = chr;
	data32[1] = 0;

	data_len = 8;
	character_ct = 1;
}

template <>
string<utf32>::string(wchar_t chr)
{
	data = new uint8_t[4];
	uint32_t* data32 = (uint32_t*)data;

	data32[0] = chr;
	data32[1] = 0;

	data_len = 8;
	character_ct = 1;
}

template <>
string<utf32>::string(const char* str)
{
	if (str)
	{
		size_t len = 0;
		while (str[len]) len++;

		character_ct = len;
		data = new uint8_t[(++len)<<2];
		data_len = len<<2;

		uint32_t* data32 = (uint32_t*)data;

		for (size_t i=0; i<len; i++)
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
string<utf32>::string(const wchar_t* str)
{
	if (str)
	{
		size_t len = 0;
		while (str[len]) len++;

		character_ct = len;
		data = new uint8_t[(++len)<<2];
		data_len = len<<2;

		uint32_t* data32 = (uint32_t*)data;

		for (size_t i=0; i<len; i++)
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
string<utf32>::string(const string<ascii>& other)
{
	data_len = other.data_len << 2;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];
	uint32_t* data32 = (uint32_t*)data;

	for (size_t i=0; i<=character_ct; i++)
		data32[i] = other.data[i];
}

template <>
const uint8_t* string<utf32>::cstring() const
{
	return 0;
}

template <>
const uint16_t* string<utf32>::nstring() const
{
	return 0;
}

template <>
const uint32_t* string<utf32>::wstring() const
{
	return (uint32_t*)data;
}
