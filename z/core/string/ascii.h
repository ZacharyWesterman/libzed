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

///analyzers
template <>
int string<ascii>::count(const string<ascii>& other) const
{
	if (!other.character_ct) return 0;

	size_t occurrence = 0;

	size_t other_i = 0;
	for (size_t i=0; i<character_ct; i++)
	{
		if (data[i] == other.data[other_i])
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
int string<ascii>::findAfter(const string<ascii>& other, size_t index, int occurrence) const
{
	if (!other.character_ct || (occurrence < 1)) return -1;

	size_t other_i = 0;
	for (size_t i=index; i<character_ct; i++)
	{
		if (data[i] == other.data[other_i])
		{
			other_i++;
			if (other_i >= other.character_ct)
			{
				occurrence--;
				other_i = 0;
			}

			if (!occurrence) return (i - other.character_ct + 1);
		}
		else
			other_i = 0;
	}

	return -1;
}

template <>
int string<ascii>::find(const string<ascii>& other, int occurrence) const
{
	return this->findAfter(other,0,occurrence);
}

template <>
int string<ascii>::findBefore(const string<ascii>& other, size_t index, int occurrence) const
{
	if (!other.character_ct || (occurrence < 1)) return -1;

	if (index > (character_ct - other.character_ct))
		index = character_ct - other.character_ct;

	size_t other_i = other.character_ct - 1;
	for (size_t i=index; i<character_ct; i--)
	{
		if (data[i] == other.data[other_i])
		{

			if (!other_i)
			{
				occurrence--;

				if (occurrence && (i < other.character_ct)) i = 0;
				other_i = other.character_ct - 1;
			}
			else
				other_i--;

			if (!occurrence) return i;
		}
		else
		{
			if (occurrence && (i < other.character_ct)) return -1;
			other_i = other.character_ct - 1;
		}
	}

	return -1;
}

template <>
int string<ascii>::findLast(const string<ascii>& other, int occurrence) const
{
	return this->findBefore(other, this->character_ct, occurrence);
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

template <>
const string<ascii>& string<ascii>::insert(const string<ascii>& other, size_t index)//insert before index
{
	if (!other.character_ct) return *this;

	if (index >= character_ct) index = character_ct;

	size_t start = index + other.character_ct;
	size_t end = character_ct + other.character_ct;
	this->increase(end);

	for (size_t i=end-1; i>=start; i--)
	{
		data[i] = data[i - other.character_ct];
	}

	end = index + other.character_ct;
	for (size_t i=index; i<end; i++)
	{
		data[i] = other.data[i-index];
	}

	character_ct += other.character_ct;
	data[character_ct] = 0;

	return *this;
}

template <>
const string<ascii>& string<ascii>::remove(size_t index, int count)
{
	if (count)
	{
		if (index >= character_ct) return *this;

		size_t start, end, offset;

		if (count < 0)
		{
			if ((index + count) > character_ct)
			{
				start = 0;
				offset = index;
			}
			else
			{
				start = index + count;
				offset = -count;
			}
		}
		else
		{
			if ((size_t)count > (character_ct - index))
				offset = character_ct - index - 1;
			else
				offset = count;

			start = index;
		}


		end = character_ct - offset;
		for (size_t i=start; i<end; i++)
		{
			data[i] = data[i+offset];
		}

		data[end] = 0;
		character_ct = end;
	}

	return *this;
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
