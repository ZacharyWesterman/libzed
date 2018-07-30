#pragma once

static bool rgxmci(const core::array<rgxll*>& list, size_t index, rgxerr& error) // (?i)
{
	if (list.isValid(index+3))
	{
		if (list[index]->id() != RGX_LPAREN) return false;
		if (list[index+1]->id() != RGX_QUERY) return false;
		if (list[index+2]->id() != RGX_SYMBOL) return false;
		if (list[index+2]->beg() != 'i') return false;
		if (list[index+2]->min() != 1) return false;
		if (list[index+2]->max() != 1) return false;
		if (list[index+3]->id() != RGX_RPAREN) return false;
	}
	else return false;

	return true;
}

static bool rgxmcni(const core::array<rgxll*>& list, size_t index, rgxerr& error) // (?!i)
{
	if (list.isValid(index+4))
	{
		if (list[index]->id() != RGX_LPAREN) return false;
		if (list[index+1]->id() != RGX_QUERY) return false;
		if (list[index+2]->id() != RGX_BANG) return false;
		if (list[index+3]->id() != RGX_SYMBOL) return false;
		if (list[index+3]->beg() != 'i') return false;
		if (list[index+3]->min() != 1) return false;
		if (list[index+3]->max() != 1) return false;
		if (list[index+4]->id() != RGX_RPAREN) return false;
	}
	else return false;

	return true;
}

static bool rgxmr(const core::array<rgxll*>& list, size_t index, rgxerr& error) // [..X-Y..]
{
	if (list.isValid(index+3))
	{
		if (list[index]->id() != RGX_SYMBOL) return false;
		if (list[index+1]->id() != RGX_DASH) return false;
		if (list[index+2]->id() != RGX_SYMBOL) return false;
	}
	else return false;

	return true;
}

static bool rgxmol(const core::array<rgxll*>& list, size_t index, rgxerr& error) // [...]
{
	if (list.isValid(index+2))
	{
		if (list[index]->id() != RGX_LBRACKET) return false;

		size_t i = index+1;
		while (list[i]->id() != RGX_RBRACKET)
		{
			bool bad = true;
			bad &= (list[i]->id() != RGX_SYMBOL);
			bad &= (list[i]->id() != RGX_NOT);
			bad &= ((list[i]->id() > RGX_NOT_PUNCT) || (list[i]->id() < RGX_SPACE));

			if (bad) return false;
			i++;
		}
	}
	else return false;

	return true;
}

static bool rgxmh(const core::array<rgxll*>& list, size_t index, rgxerr& error) // \xFF
{
	if (list.isValid(index+2))
	{
		if (list[index]->id() != RGX_HEXDIGIT) return false;
		if (list[index+1]->id() != RGX_SYMBOL)
		{
			error = RGX_BAD_HEX;
			return true;
		}
		if (!core::isNumeric(list[index+1]->beg(), 16))
		{
			error = RGX_BAD_HEX;
			return true;
		}
		if (list[index+2]->id() != RGX_SYMBOL)
		{
			error = RGX_BAD_HEX;
			return true;
		}
		if (!core::isNumeric(list[index+2]->beg(), 16))
		{
			error = RGX_BAD_HEX;
			return true;
		}
	}
	else return false;

	return true;
}

//root is dynamically allocated
rgxerr rgxlex(const core::array<rgxss>& input, rgxll*& root)
{
	core::array<rgxll*> list;
	for (size_t i=0; i<input.length(); i++)
		list.add(new rgxll(input[i]));

	bool madeChange = true;
	rgxerr error = RGX_NO_ERROR;

	while (madeChange)
	{
		madeChange = false;

		for (size_t i=0; i<list.length(); i++)
		{
			if (rgxmci(list,i, error)) //begin case-insensitive
			{
				if (!error)
				{
					list[i]->setID(RGX_CASEI);
					delete list[i+1];
					delete list[i+2];
					delete list[i+3];
					list.remove(i+1, 3);
					madeChange = true;
				}
			}
			else if (rgxmcni(list,i,error)) //end case-insensitive
			{
				if (!error)
				{
					list[i]->setID(RGX_NOT_CASEI);
					for (size_t k=i+1; k<(i+4); k++) delete list[k];
					list.remove(i+1, 4);
					madeChange = true;
				}
			}
			else if (rgxmr(list,i,error)) //character range
			{
				if (!error)
				{
					list[i]->setRange(list[i]->beg(), list[i+2]->beg());
					delete list[i+1];
					delete list[i+2];
					list.remove(i+1, 2);
					madeChange = true;
				}
			}
			else if (rgxmol(list,i,error))
			{
				if (!error)
				{
					list[i]->setID(RGX_OR_LIST);
					size_t k = i+1;
					while (list[k]->id() != RGX_RBRACKET)
					{
						list[i]->children.add(list[k]);
						k++;
					}
					delete list[k];
					list.remove(i+1,k-i);
					madeChange = true;
				}
			}
			else if (rgxmh(list,i,error)) // \xFF
			{
				if (!error)
				{
					uint32_t c[2];
					c[0] = core::numeralValue(list[i+1]->beg());
					c[1] = core::numeralValue(list[i+2]->beg());
					list[i]->setID(RGX_SYMBOL);
					list[i]->setExact((c[0] << 4) + c[1]);
					delete list[i+1];
					delete list[i+2];
					list.remove(i+1, 2);
					madeChange = true;
				}
			}

			if (error)
			{
				madeChange = false;
				break;
			}
		}
	}

	if (list.length() > 1)
	{
		root = new rgxll(RGX_AND_LIST);
		for (size_t i=0; i<list.length(); i++)
			root->children.add(list[i]);
	}
	else if (list.length())
	{
		root = list[0];
	}
	else error = RGX_LEX_FAIL;

	return error;
}
