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
string<utf32>::string(const uint32_t& chr)
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
	data_len = (other.character_ct + 1) << 2;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];
	uint32_t* data32 = (uint32_t*)data;

	for (size_t i=0; i<=character_ct; i++)
		data32[i] = other.data[i];
}

template <>
string<utf32>::string(const string<utf8>& other)
{
	character_ct = 0;
	for (size_t i=0; i<other.character_ct; i+=lenFromUTF8(&other.data[i]))
		character_ct++;

	data_len = (character_ct + 1) << 2;
	data = new uint8_t[data_len];

	uint32_t* data32 = (uint32_t*)data;
	data32[character_ct] = 0;

	size_t pos = 0;
	for (size_t i=0; i<character_ct; i++)
	{
		uint32_t val = fromUTF8(&other.data[pos]);
		pos += lenFromUTF8(&other.data[pos]);

		data32[i] = val;
	}
}

template <>
string<utf32>::string(const string<utf16>& other)
{
	data_len = (other.character_ct + 1) << 2;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];

	uint32_t* data32 = (uint32_t*)data;
	uint16_t* data16 = (uint16_t*)other.data;

	for (size_t i=0; i<=character_ct; i++)
		data32[i] = data16[i];
}

template <>
string<utf32>::string(const string<utf32>& other)
{
	data_len = (other.character_ct + 1) << 2;
	character_ct = other.character_ct;

	data = new uint8_t[data_len];

	uint32_t* data32 = (uint32_t*)data;
	uint32_t* other32 = (uint32_t*)other.data;

	for (size_t i=0; i<=character_ct; i++)
		data32[i] = other32[i];
}

template <>
const uint32_t string<utf32>::at(size_t index) const
{
	uint32_t* data32 = (uint32_t*)data;

	if (index < character_ct)
		return data32[index];
	else
		return 0;
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

template <>
void string<utf32>::initChar(uint32_t chr, size_t index)
{
	uint32_t* data32 = (uint32_t*)data;
	data32[index] = chr;
}

template <>
size_t string<utf32>::charSize() const
{
	return 4;
}

///operators
template <>
const string<utf32>& string<utf32>::operator+=(const string<utf32>& other)
{
	size_t new_size = character_ct + other.character_ct + 1;
	this->increase(new_size << 2);

	uint32_t* data32 = (uint32_t*)data;
	uint32_t* other32 = (uint32_t*)other.data;

	for (size_t i=0; i<other.character_ct; i++)
	{
		data32[character_ct + i] = other32[i];
	}
	data32[new_size-1] = 0;

	character_ct += other.character_ct;

	return *this;
}

///analyzers
template <>
int string<utf32>::count(const string<utf32>& other) const
{
	if (!other.character_ct) return 0;

	uint32_t* data32 = (uint32_t*)data;
	uint32_t* other32 = (uint32_t*)other.data;

	size_t occurrence = 0;

	size_t other_i = 0;
	for (size_t i=0; i<character_ct; i++)
	{
		if (data32[i] == other32[other_i])
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
int string<utf32>::findAfter(const string<utf32>& other, size_t index, int occurrence) const
{
	if (!other.character_ct || (occurrence < 1)) return -1;

	uint32_t* data32 = (uint32_t*)data;
	uint32_t* other32 = (uint32_t*)other.data;

	size_t other_i = 0;
	for (size_t i=index; i<character_ct; i++)
	{
		//reset to first char of other if not still matching
		if (data32[i] != other32[other_i])
			other_i = 0;

		if (data32[i] == other32[other_i])
		{
			other_i++;
			if (other_i >= other.character_ct)
			{
				occurrence--;
				other_i = 0;
			}

			if (!occurrence) return (i - other.character_ct + 1);
		}
	}

	return -1;
}

template <>
int string<utf32>::findBefore(const string<utf32>& other, size_t index, int occurrence) const
{
	if (!other.character_ct || (occurrence < 1)) return -1;

	uint32_t* data32 = (uint32_t*)data;
	uint32_t* other32 = (uint32_t*)other.data;

	if (index > (character_ct - other.character_ct))
		index = character_ct - other.character_ct;

	size_t other_i = other.character_ct - 1;
	for (size_t i=index; i<character_ct; i--)
	{
		if (data32[i] == other32[other_i])
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

///mutators
template <>
string<utf32> string<utf32>::substr(size_t index, int count) const
{
	string<utf32> result;
	uint32_t* data32 = (uint32_t*)data;
	uint32_t* result32;

	if (count < 0)
	{
		if (index >= character_ct) index = character_ct - 1;

		count = -count;
		if ((size_t)count > index+1) count = index+1;
		result.increase((1+count) << 2);

		result32 = (uint32_t*)result.data;

		size_t beg = index - count + 1;
		for (size_t i=beg; i<=index; i++)
		{
			result32[index-i] = data32[i];
		}
		result32[count] = 0;
		result.character_ct = count;
	}
	else if (count)
	{
		if (index >= character_ct) return result;

		if ((size_t)count > index+1) count = index+1;
		result.increase((1+count) << 2);

		result32 = (uint32_t*)result.data;

		size_t end = index + count;
		for (size_t i=index; i<end; i++)
			result32[i-index] = data32[i];

		result32[count] = 0;
		result.character_ct = count;
	}

	return result;
}

template <>
const string<utf32>& string<utf32>::insert(const string<utf32>& other, size_t index)//insert before index
{
	if (!other.character_ct) return *this;

	if (index >= character_ct) index = character_ct;

	size_t start = index + other.character_ct;
	size_t end = character_ct + other.character_ct;
	this->increase(end << 2);

	uint32_t* data32 = (uint32_t*)data;
	uint32_t* other32 = (uint32_t*)other.data;

	for (size_t i=end-1; i>=start; i--)
	{
		data32[i] = data32[i - other.character_ct];
	}

	end = index + other.character_ct;
	for (size_t i=index; i<end; i++)
	{
		data32[i] = other32[i-index];
	}

	character_ct += other.character_ct;
	data32[character_ct] = 0;

	return *this;
}

template <>
const string<utf32>& string<utf32>::remove(size_t index, int count)
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

		uint32_t* data32 = (uint32_t*)data;

		end = character_ct - offset;
		for (size_t i=start; i<end; i++)
		{
			data32[i] = data32[i+offset];
		}

		data32[end] = 0;
		character_ct = end;
	}

	return *this;
}

template <>
const string<utf32>& string<utf32>::replace(size_t index, int count, const string<utf32>& other)
{
	if (count)
	{
		size_t start, end;

		if (count < 0)
		{
			if ((index >= character_ct) && ((size_t)-count >= character_ct))
				return operator=(other);

			if (index >= character_ct)
			{
				start = character_ct + count;
				end = character_ct;
			}
			else
			{
				end = index + 1;

				if ((size_t)-count >= character_ct)
					start = 0;
				else
					start = end + count;
			}
		}
		else
		{
			if (index >= character_ct)
				return operator+=(other);

			if (!index && ((size_t)count >= character_ct))
				return operator=(other);

			start = index;
			if ((size_t)count >= character_ct)
				end = character_ct;
			else
				end = start + count;
		}


		size_t offset = end - start;
		size_t newCharCt = character_ct - offset + other.character_ct;
		this->increase((newCharCt + 1) << 1);

		uint32_t* data32 = (uint32_t*)data;
		uint32_t* other32 = (uint32_t*)other.data;

		if (newCharCt < character_ct)
		{
			//pull chars in
			size_t toOffs = newCharCt - character_ct;

			for (size_t i=end; i<character_ct; i++)
				data32[i+toOffs] = data32[i];
		}
		else if (newCharCt > character_ct)
		{
			//pull chars out
			size_t toPos = newCharCt + 1;
			size_t fromPos = character_ct + 1;

			for (size_t i=end; i<character_ct; i++)
				data32[toPos-i] = data32[fromPos-i];
		}
		//else just directly replace chars

		for (size_t i=0; i<other.character_ct; i++)
			data32[i+start] = other32[i];

		character_ct = newCharCt;
		data32[character_ct] = 0;
	}

	return *this;
}
