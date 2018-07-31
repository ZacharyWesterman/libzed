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
		if (list[index+1]->id() != RGX_DASH) return false;
		if (list[index]->id() != RGX_SYMBOL) error = RGX_BAD_RANGE;
		if (list[index+2]->id() != RGX_SYMBOL) error = RGX_BAD_RANGE;
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
			bad &= ((list[i]->id() > RGX_END) || (list[i]->id() < RGX_BEGIN));

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

static bool rgxmal(const core::array<rgxll*>& list, size_t index, rgxerr& error) // (...)
{
	if (list.isValid(index+1))
	{
		if (list[index]->id() != RGX_LPAREN) return false;

		size_t i = index+1;
		while (list[i]->id() != RGX_RPAREN)
		{
			if (list[i]->id() == RGX_LPAREN) return false;

			bool bad = true;
			bad &= (list[i]->id() != RGX_SYMBOL);
			bad &= (list[i]->id() != RGX_OR_LIST);
			bad &= (list[i]->id() != RGX_AND_LIST);
			bad &= (list[i]->id() != RGX_COLUMN);
			bad &= ((list[i]->id() > RGX_END) || (list[i]->id() < RGX_BEGIN));

			if (bad) return false;
			i++;
		}
	}
	else return false;

	return true;
}

static bool rgxmali(const core::array<rgxll*>& list, size_t index, rgxerr& error) // match if total regex is valid, else error
{
	size_t i = index;
	while (list.isValid(i))
	{
		bool bad = true;
		bad &= (list[i]->id() != RGX_SYMBOL);
		bad &= (list[i]->id() != RGX_OR_LIST);
		bad &= (list[i]->id() != RGX_AND_LIST);
		bad &= (list[i]->id() != RGX_COLUMN);
		bad &= ((list[i]->id() > RGX_END) || (list[i]->id() < RGX_BEGIN));

		if (bad)
		{
			error = RGX_LEX_FAIL;
			return false;
		}
		i++;
	}

	return true;
}

static bool rgxmcc(const core::array<rgxll*>& list, size_t index, rgxerr& error) // match if can count
{
	bool noMatch = true;
	noMatch &= (list[index]->id() != RGX_ASTERISK);
	noMatch &= (list[index]->id() != RGX_PLUS);
	noMatch &= (list[index]->id() != RGX_COUNT);
	if (noMatch) return false;

	if (!index || (list[index-1]->id() == RGX_LPAREN))
	{
		error = RGX_BAD_COUNT_LOC;
		return true;
	}

	if (list[index-1]->id() == RGX_SYMBOL) return true;
	if (list[index-1]->id() == RGX_OR_LIST) return true;
	if (list[index-1]->id() == RGX_AND_LIST) return true;

	if (list[index-1]->id() > RGX_END) return false;
	if (list[index-1]->id() < RGX_BEGIN) return false;

	return true;
}

//root is dynamically allocated
rgxerr rgxlex(const core::array<rgxss>& input, rgxll*& root)
{
	root = 0;

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
			if (rgxmcc(list,i,error))//* or + or {..}
			{
				if (!error)
				{
					if (list[i]->id() == RGX_ASTERISK)
					{
						list[i-1]->setCountRange(0,-1);
					}
					else if (list[i]->id() == RGX_PLUS)
					{
						list[i-1]->setCountRange(1,-1);
					}

					delete list[i];
					list.remove(i--);
				}
			}
			else if (rgxmci(list,i, error)) //begin case-insensitive
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
			else if (rgxmol(list,i,error))//[...]
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
			else if (rgxmal(list,i,error))//(...)
			{
				if (!error)
				{
					list[i]->setID(RGX_OR_LIST);

					rgxll* node = new rgxll(RGX_AND_LIST);
					size_t k = i+1;
					while (list[k]->id() != RGX_RPAREN)
					{
						if (list[k]->id() == RGX_COLUMN)
						{
							delete list[k];
							list[i]->children.add(node);
							node = new rgxll(RGX_AND_LIST);
						}
						else
						{
							node->children.add(list[k]);
						}
						k++;
					}
					delete list[k];
					list.remove(i+1,k-i);

					if (list[i]->children.length())
					{
						list[i]->children.add(node);
					}
					else
					{
						delete list[i];
						list[i] = node;
					}

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
		if (rgxmali(list,0,error))
		{
			rgxll* node = new rgxll(RGX_AND_LIST);
			size_t i = 0;
			while (list.isValid(i))
			{
				if (list[i]->id() == RGX_COLUMN)
				{
					delete list[i];
					if (!root) root = new rgxll(RGX_OR_LIST);
					root->children.add(node);
					node = new rgxll(RGX_AND_LIST);
				}
				else
				{
					node->children.add(list[i]);
				}
				i++;
			}

			if (root)
			{
				root->children.add(node);
			}
			else
			{
				root = node;
			}
		}
	}
	else if (list.length())
	{
		root = list[0];
	}
	else error = RGX_LEX_FAIL;

	return error;
}
