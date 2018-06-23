#pragma once

template <>
string<ascii>::string()
{
	data = new uint8_t[1];
	data[0] = 0;

	data_len = 1;
	character_ct = 0;
}

template <>
string<ascii>::string(char chr)
{
	data = new uint8_t[2];
	data[0] = chr;
	data[1] = 0;

	data_len = 2;
	character_ct = 1;
}

template <>
string<ascii>::string(wchar_t chr)
{
	data = new uint8_t[2];
	data[0] = (chr > 0xFF) ? '?' : chr;
	data[1] = 0;

	data_len = 2;
	character_ct = 1;
}

template <>
string<ascii>::string(const char* str)
{
	if (str)
	{
		size_t len = 0;
		while (str[len]) len++;

		character_ct = len;
		data = new uint8_t[++len];
		data_len = len;

		for (size_t i=0; i<len; i++)
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
string<ascii>::string(const wchar_t* str)
{
	if (str)
	{
		size_t len = 0;
		while (str[len]) len++;

		character_ct = len;
		data = new uint8_t[++len];
		data_len = len;

		for (size_t i=0; i<len; i++)
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
string<ascii>::string(const string<ascii>& other)
{
	data_len = other.data_len;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];

	for (size_t i=0; i<data_len; i++)
		data[i] = other.data[i];
}

template <>
const uint8_t* string<ascii>::cstring() const
{
	return data;
}

template <>
const uint16_t* string<ascii>::nstring() const
{
	return 0;
}

template <>
const uint32_t* string<ascii>::wstring() const
{
	return 0;
}
