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
string<ascii>::string(const uint32_t& chr)
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
	data_len = other.character_ct + 1;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];

	for (size_t i=0; i<data_len; i++)
		data[i] = other.data[i];
}

template <>
string<ascii>::string(const string<utf8>& other)
{
	data_len = other.character_ct + 1;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];
	data[character_ct] = 0;

	size_t pos = 0;
	for (size_t i=0; i<data_len; i++)
	{
		uint32_t val = fromUTF8(&other.data[pos]);
		pos += lenFromUTF8(&other.data[pos]);

		data[i] = (val > 0xFF) ? '?' : val;
	}
}

template <>
string<ascii>::string(const string<utf16>& other)
{
	data_len = other.character_ct + 1;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];
	data[character_ct] = 0;

	uint16_t* data16 = (uint16_t*)other.data;

	for (size_t i=0; i<data_len; i++)
	{
		data[i] = (data16[i] > 0xFF) ? '?' : data16[i];
	}
}

template <>
string<ascii>::string(const string<utf32>& other)
{
	data_len = other.character_ct + 1;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];
	data[character_ct] = 0;

	uint32_t* data32 = (uint32_t*)other.data;

	for (size_t i=0; i<data_len; i++)
		data[i] = (data32[i] > 0xFF) ? '?' : data32[i];
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

template <>
void string<ascii>::initChar(uint32_t chr, size_t index)
{
	data[index] = chr;
}

template <>
size_t string<ascii>::charSize() const
{
	return 1;
}

template <>
const uint32_t string<ascii>::at(size_t index) const
{
	if (index < character_ct)
		return data[index];
	else
		return 0;
}

///mutators
template <>
string<ascii> string<ascii>::substr(size_t index, int count) const
{
	string<ascii> result;

	if (count < 0)
	{
		if (index >= character_ct) index = character_ct - 1;

		count = -count;
		if ((size_t)count > index+1) count = index+1;
		result.increase(1+count);

		size_t beg = index - count + 1;
		for (size_t i=beg; i<=index; i++)
		{
			result.data[index-i] = data[i];
		}
		result.data[count] = 0;
		result.character_ct = count;
	}
	else if (count)
	{
		if (index >= character_ct) return result;

		if ((size_t)count > index+1) count = index+1;
		result.increase(1+count);

		size_t end = index + count;
		for (size_t i=index; i<end; i++)
			result.data[i-index] = data[i];

		result.data[count] = 0;
		result.character_ct = count;
	}

	return result;
}

///operators

template <>
const string<ascii>& string<ascii>::operator+=(const string<ascii>& other)
{
	size_t new_size = character_ct + other.character_ct + 1;
	this->increase(new_size);

	for (size_t i=0; i<other.character_ct; i++)
	{
		data[character_ct + i] = other.data[i];
	}
	data[new_size-1] = 0;

	character_ct += other.character_ct;

	return *this;
}
