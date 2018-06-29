#pragma once

template <>
string<utf16>::string()
{
	data = new uint8_t[2];
	*((uint16_t*)data) = 0;

	data_len = 2;
	character_ct = 0;
}

template <>
string<utf16>::string(char chr)
{
	data = new uint8_t[4];
	uint16_t* data16 = (uint16_t*)data;

	data16[0] = chr;
	data16[1] = 0;

	data_len = 4;
	character_ct = 1;
}

template <>
string<utf16>::string(wchar_t chr)
{
	data = new uint8_t[4];
	uint16_t* data16 = (uint16_t*)data;

	data16[0] = (chr > 0xFFFF) ? '?' : chr;
	data16[1] = 0;

	data_len = 4;
	character_ct = 1;
}

template <>
string<utf16>::string(const uint32_t& chr)
{
	data = new uint8_t[4];
	uint16_t* data16 = (uint16_t*)data;

	data16[0] = (chr > 0xFFFF) ? '?' : chr;
	data16[1] = 0;

	data_len = 4;
	character_ct = 1;
}

template <>
string<utf16>::string(const char* str)
{
	if (str)
	{
		size_t len = 0;
		while (str[len]) len++;

		character_ct = len;
		data = new uint8_t[(++len)<<1];
		data_len = len<<1;

		uint16_t* data16 = (uint16_t*)data;

		for (size_t i=0; i<len; i++)
			data16[i] = str[i];
	}
	else
	{
		data = new uint8_t[2];
		*((uint16_t*)data) = 0;

		data_len = 2;
		character_ct = 0;
	}
}

template <>
string<utf16>::string(const wchar_t* str)
{
	if (str)
	{
		size_t len = 0;
		while (str[len]) len++;

		character_ct = len;
		data = new uint8_t[(++len)<<1];
		data_len = len<<1;

		uint16_t* data16 = (uint16_t*)data;

		for (size_t i=0; i<len; i++)
		{
			if (str[i] > 0xFFFF)
				data16[i] = '?';
			else
				data16[i] = str[i];
		}
	}
	else
	{
		data = new uint8_t[2];
		*((uint16_t*)data) = 0;

		data_len = 2;
		character_ct = 0;
	}
}

template <>
string<utf16>::string(const string<ascii>& other)
{
	data_len = (other.character_ct + 1) << 1;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];
	uint16_t* data16 = (uint16_t*)data;

	for (size_t i=0; i<=character_ct; i++)
		data16[i] = other.data[i];
}

template <>
string<utf16>::string(const string<utf8>& other)
{
	character_ct = 0;
	for (size_t i=0; i<other.character_ct; i+=lenFromUTF8(&other.data[i]))
		character_ct++;

	data_len = (character_ct + 1) << 1;
	data = new uint8_t[data_len];

	uint16_t* data16 = (uint16_t*)data;
	data16[character_ct] = 0;

	size_t pos = 0;
	for (size_t i=0; i<=character_ct; i++)
	{
		uint32_t val = fromUTF8(&other.data[pos]);
		pos += lenFromUTF8(&other.data[pos]);

		data16[i] = (val > 0xFFFF) ? '?' : val;
	}
}

template <>
string<utf16>::string(const string<utf16>& other)
{
	data_len = (other.character_ct + 1) << 1;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];

	uint16_t* data16 = (uint16_t*)data;
	uint16_t* other16 = (uint16_t*)other.data;

	for (size_t i=0; i<=character_ct; i++)
		data16[i] = other16[i];
}

template <>
string<utf16>::string(const string<utf32>& other)
{
	data_len = (other.character_ct + 1) << 1;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];

	uint16_t* data16 = (uint16_t*)data;
	uint32_t* data32 = (uint32_t*)other.data;

	for (size_t i=0; i<=character_ct; i++)
		data16[i] = (data32[i] > 0xFFFF) ? '?' : data32[i];
}

template <>
const uint32_t string<utf16>::at(size_t index) const
{
	uint16_t* data16 = (uint16_t*)data;

	if (index < character_ct)
		return data16[index];
	else
		return 0;
}

template <>
const uint8_t* string<utf16>::cstring() const
{
	return 0;
}

template <>
const uint16_t* string<utf16>::nstring() const
{
	return (uint16_t*)data;
}

template <>
const uint32_t* string<utf16>::wstring() const
{
	return 0;
}

template <>
void string<utf16>::initChar(uint32_t chr, size_t index)
{
	uint16_t* data16 = (uint16_t*)data;
	data16[index] = chr;
}

template <>
size_t string<utf16>::charSize() const
{
	return 2;
}

///analyzers
template <>
int string<utf16>::count(const string<utf16>& other) const
{
	if (!other.character_ct) return 0;

	uint16_t* data16 = (uint16_t*)data;
	uint16_t* other16 = (uint16_t*)other.data;

	size_t occurrence = 0;

	size_t other_i = 0;
	for (size_t i=0; i<character_ct; i++)
	{
		if (data16[i] == other16[other_i])
		{
			other_i++;
			if (other_i >= other.character_ct)
				occurrence++;

			if (!occurrence) return (i - other_i + 1);
		}
		else
		{
			if ((character_ct - i) <= other.character_ct)
				return occurrence;

			other_i = 0;
		}
	}

	return occurrence;
}

template <>
int string<utf16>::find(const string<utf16>& other, int occurrence) const
{
	if (!other.character_ct || (occurrence < 1)) return -1;

	uint16_t* data16 = (uint16_t*)data;
	uint16_t* other16 = (uint16_t*)other.data;

	size_t other_i = 0;
	for (size_t i=0; i<character_ct; i++)
	{
		if (data16[i] == other16[other_i])
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
string<utf16> string<utf16>::substr(size_t index, int count) const
{
	string<utf16> result;
	uint16_t* data16 = (uint16_t*)data;
	uint16_t* result16;

	if (count < 0)
	{
		if (index >= character_ct) index = character_ct - 1;

		count = -count;
		if ((size_t)count > index+1) count = index+1;
		result.increase((1+count) << 1);

		result16 = (uint16_t*)result.data;

		size_t beg = index - count + 1;
		for (size_t i=beg; i<=index; i++)
		{
			result16[index-i] = data16[i];
		}
		result16[count] = 0;
		result.character_ct = count;
	}
	else if (count)
	{
		if (index >= character_ct) return result;

		if ((size_t)count > index+1) count = index+1;
		result.increase((1+count) << 1);

		result16 = (uint16_t*)result.data;

		size_t end = index + count;
		for (size_t i=index; i<end; i++)
			result16[i-index] = data16[i];

		result16[count] = 0;
		result.character_ct = count;
	}

	return result;
}

///operators

template <>
const string<utf16>& string<utf16>::operator+=(const string<utf16>& other)
{
	size_t new_size = character_ct + other.character_ct + 1;
	this->increase(new_size << 1);

	uint16_t* data16 = (uint16_t*)data;
	uint16_t* other16 = (uint16_t*)other.data;

	for (size_t i=0; i<other.character_ct; i++)
	{
		data16[character_ct + i] = other16[i];
	}
	data16[new_size-1] = 0;

	character_ct += other.character_ct;

	return *this;
}
