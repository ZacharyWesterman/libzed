#pragma once

template <>
string<utf8>::string()
{
	data = new uint8_t[1];
	data[0] = 0;

	data_len = 1;
	character_ct = 0;
}

template <>
string<utf8>::string(char chr)
{
	data = new uint8_t[2];
	data[0] = chr;
	data[1] = 0;

	data_len = 2;
	character_ct = 1;
}

template <>
string<utf8>::string(wchar_t chr)
{
	data = new uint8_t[5];
	data_len = 5;

	int len = toUTF8(data, chr);
	data[len] = 0;

	character_ct = 1;
}

template <>
string<utf8>::string(const char* str)
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
string<utf8>::string(const wchar_t* str)
{
	if (str)
	{
		size_t i = 0;
		size_t len = 0;

		while (str[i])
			len += lenToUTF8(str[i++]);

		character_ct = i-1;

		data = new uint8_t[len+1];
		data[len] = 0;
		data_len = len+1;

		size_t pos = 0;
		for (size_t i=0; i<=character_ct; i++)
		{
			pos += toUTF8(&data[pos], str[i]);
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
string<utf8>::string(const string<ascii>& other)
{
	data_len = 1;
	character_ct = other.character_ct;

	for (size_t i=0; i<other.data_len; i++)
		data_len += lenToUTF8(other.data[i]);

	data = new uint8_t[data_len];
	data[data_len-1] = 0;

	size_t len = 0;
	for (size_t i=0; i<character_ct; i++)
		len += toUTF8(&data[len], other.data[i]);
}

template <>
const uint8_t* string<utf8>::cstring() const
{
	return data;
}

template <>
const uint16_t* string<utf8>::nstring() const
{
	return 0;
}

template <>
const uint32_t* string<utf8>::wstring() const
{
	return 0;
}
