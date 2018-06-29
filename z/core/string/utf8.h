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
string<utf8>::string(const uint32_t& chr)
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

		character_ct = len;

		data_len = character_ct + 1;
		data = new uint8_t[data_len];
		data[character_ct] = 0;

		size_t pos = 0;
		for (size_t j=0; j<i; j++)
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

	character_ct = data_len - 1;
}

template <>
string<utf8>::string(const string<utf8>& other)
{
	data_len = other.data_len;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];

	for (size_t i=0; i<data_len; i++)
		data[i] = other.data[i];
}

template <>
string<utf8>::string(const string<utf16>& other)
{
	data_len = 1;
	character_ct = other.character_ct;

	uint16_t* data16 = (uint16_t*)other.data;

	for (size_t i=0; i<=other.character_ct; i++)
		data_len += lenToUTF8(data16[i]);

	data = new uint8_t[data_len];
	data[data_len-1] = 0;

	size_t len = 0;
	for (size_t i=0; i<character_ct; i++)
		len += toUTF8(&data[len], data16[i]);

	character_ct = len;
}

template <>
string<utf8>::string(const string<utf32>& other)
{
	data_len = 1;
	character_ct = other.character_ct;

	uint32_t* data32 = (uint32_t*)other.data;

	for (size_t i=0; i<=other.character_ct; i++)
		data_len += lenToUTF8(data32[i]);

	data = new uint8_t[data_len];
	data[data_len-1] = 0;

	size_t len = 0;
	for (size_t i=0; i<character_ct; i++)
		len += toUTF8(&data[len], data32[i]);

	character_ct = len;
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

template <>
void string<utf8>::initChar(uint32_t chr, size_t index)
{
	data[index] = chr;
}

template <>
size_t string<utf8>::charSize() const
{
	return 1;
}

template <>
size_t string<utf8>::chars() const
{
	size_t i = 0;
	size_t result = 0;

	while (i < character_ct)
	{
		i += lenFromUTF8(&data[i]);
		result++;
	}

	return result;
}

template <>
const uint32_t string<utf8>::at(size_t index) const
{
	if (index < character_ct)
		return fromUTF8(&data[index]);
	else
		return 0;
}

///analyzers
template <>
int string<utf8>::find(const string<utf8>& other, int occurrence) const
{
	if (occurrence < 1) return -1;

	size_t other_i = 0;
	for (size_t i=0; i<character_ct; i++)
	{
		if (data[i] == other.data[other_i])
		{
			other_i++;
			if (other_i >= other.character_ct)
				occurrence--;

			if (!occurrence) return (i - other_i + 1);
		}
		else
			other_i = 0;
	}

	return -1;
}

///mutators
template <>
string<utf8> string<utf8>::substr(size_t index, int count) const
{
	return string<utf8>(string<utf32>(*this).substr(index,count));
}

//operators

template <>
const string<utf8>& string<utf8>::operator+=(const string<utf8>& other)
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
