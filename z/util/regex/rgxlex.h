#pragma once

static bool rgxmpf(const core::array<rgxll*>& list, size_t index, rgxerr& error) // (?.)
{
	if (list.isValid(index+3))
	{
		if (list[index]->id() != RGX_LPAREN) return false;
		if (list[index+1]->id() != RGX_QUERY) return false;
		if (list[index+2]->id() == RGX_BANG) return false;
		if (list[index+2]->id() == RGX_PREVIOUS) return false;
		if (list[index+2]->id() == RGX_EQUALS) return false;
		if (list[index+2]->id() == RGX_DASH) return false;
		if (list[index+2]->id() != RGX_SYMBOL)
		{
			error = RGX_BAD_POS_FLAG;
			return true;
		}
		if (list[index+3]->id() != RGX_RPAREN)
		{
			error = RGX_BAD_POS_FLAG;
			return true;
		}

		uint32_t ch = list[index+2]->beg();
		if (ch == 'i') return true;
		if (ch == 's') return true;
	}
	else return false;

	//if we get here, no good flags have been identified
	error = RGX_BAD_POS_FLAG;
	return true;
}

static bool rgxmnf(const core::array<rgxll*>& list, size_t index, rgxerr& error) // (?-.)
{
	if (list.isValid(index+4))
	{
		if (list[index]->id() != RGX_LPAREN) return false;
		if (list[index+1]->id() != RGX_QUERY) return false;
		if (list[index+2]->id() != RGX_DASH) return false;
		if (list[index+3]->id() != RGX_SYMBOL)
		{
			error = RGX_BAD_NEG_FLAG;
			return true;
		}
		if (list[index+4]->id() != RGX_RPAREN)
		{
			error = RGX_BAD_NEG_FLAG;
			return true;
		}

		uint32_t ch = list[index+3]->beg();
		if (ch == 'i') return true;
		if (ch == 's') return true;
	}
	else return false;

	//if we get here, no good flags have been identified
	error = RGX_BAD_NEG_FLAG;
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

static bool rgxmq(const core::array<rgxll*>& list, size_t index, rgxerr& error) // ? (match if can set not greedy)
{
	if (list[index]->id() != RGX_GREEDY) return false;

	if (!index)
	{
		error = RGX_BAD_GREED_LOC;
		return true;
	}

	bool bad = true;
	bad &= (list[index-1]->id() != RGX_SYMBOL);
	bad &= (list[index-1]->id() != RGX_OR_LIST);
	bad &= (list[index-1]->id() != RGX_AND_LIST);
	bad &= (list[index-1]->id() != RGX_COLUMN);
	bad &= ((list[index-1]->id() > RGX_END) || (list[index-1]->id() < RGX_BEGIN));
	if (bad) return false;

	if (list[index-1]->max() < (size_t)-1)
	{
		error = RGX_GREED_INEFFECTIVE;
	}

	return true;
}

static bool rgxmpla(const core::array<rgxll*>& list, size_t index, rgxerr& error) // (?=..)
{
	if (list.isValid(index+3))
	{
		if (list[index]->id() != RGX_LPAREN) return false;
		if (list[index+1]->id() != RGX_QUERY) return false;
		if (list[index+2]->id() != RGX_EQUALS) return false;

		if (list[index+3]->id() == RGX_RPAREN)
		{
			error = RGX_BAD_LOOKAHEAD;
		}

		size_t i = index+3;
		while (list[i]->id() != RGX_RPAREN)
		{
			if (list[i]->id() == RGX_LBRACKET) return false;

			bool bad = true;
			bad &= (list[i]->id() != RGX_SYMBOL);
			bad &= (list[i]->id() != RGX_OR_LIST);
			bad &= ((list[i]->id() > RGX_END) || (list[i]->id() < RGX_BEGIN));

			if (bad)
			{
				error = RGX_BAD_LOOKAHEAD;
				return true;
			}
			i++;
		}
	}
	else return false;

	return true;
}

static bool rgxmnla(const core::array<rgxll*>& list, size_t index, rgxerr& error) // (?!..)
{
	if (list.isValid(index+3))
	{
		if (list[index]->id() != RGX_LPAREN) return false;
		if (list[index+1]->id() != RGX_QUERY) return false;
		if (list[index+2]->id() != RGX_BANG) return false;

		if (list[index+3]->id() == RGX_RPAREN)
		{
			error = RGX_BAD_LOOKAHEAD;
		}

		size_t i = index+3;
		while (list[i]->id() != RGX_RPAREN)
		{
			if (list[i]->id() == RGX_LBRACKET) return false;

			bool bad = true;
			bad &= (list[i]->id() != RGX_SYMBOL);
			bad &= (list[i]->id() != RGX_OR_LIST);
			bad &= ((list[i]->id() > RGX_END) || (list[i]->id() < RGX_BEGIN));

			if (bad)
			{
				error = RGX_BAD_LOOKAHEAD;
				return true;
			}
			i++;
		}
	}
	else return false;

	return true;
}

static bool rgxmplb(const core::array<rgxll*>& list, size_t index, rgxerr& error) // (?<..)
{
	if (list.isValid(index+3))
	{
		if (list[index]->id() != RGX_LPAREN) return false;
		if (list[index+1]->id() != RGX_QUERY) return false;
		if (list[index+2]->id() != RGX_PREVIOUS) return false;

		if (list[index+3]->id() == RGX_RPAREN)
		{
			error = RGX_BAD_LOOKBEHIND;
		}

		size_t i = index+3;
		while (list[i]->id() != RGX_RPAREN)
		{
			if (list[i]->id() == RGX_LBRACKET) return false;

			bool bad = true;
			bad &= (list[i]->id() != RGX_SYMBOL);
			bad &= (list[i]->id() != RGX_OR_LIST);
			bad &= ((list[i]->id() > RGX_END) || (list[i]->id() < RGX_BEGIN));

			if (bad)
			{
				error = RGX_BAD_LOOKBEHIND;
				return true;
			}
			i++;
		}
	}
	else return false;

	return true;
}

static bool rgxmnlb(const core::array<rgxll*>& list, size_t index, rgxerr& error) // (?<!..)
{
	if (list.isValid(index+4))
	{
		if (list[index]->id() != RGX_LPAREN) return false;
		if (list[index+1]->id() != RGX_QUERY) return false;
		if (list[index+2]->id() != RGX_PREVIOUS) return false;
		if (list[index+3]->id() != RGX_BANG) return false;

		if (list[index+4]->id() == RGX_RPAREN)
		{
			error = RGX_BAD_LOOKBEHIND;
		}

		size_t i = index+4;
		while (list[i]->id() != RGX_RPAREN)
		{
			if (list[i]->id() == RGX_LBRACKET) return false;

			bool bad = true;
			bad &= (list[i]->id() != RGX_SYMBOL);
			bad &= (list[i]->id() != RGX_OR_LIST);
			bad &= ((list[i]->id() > RGX_END) || (list[i]->id() < RGX_BEGIN));

			if (bad)
			{
				error = RGX_BAD_LOOKBEHIND;
				return true;
			}
			i++;
		}
	}
	else return false;

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
			if (list[i]->id() == RGX_LBRACE) // {..}
			{
				size_t k = i+1;
				size_t min = 0;
				size_t max = 0;

				bool minSpec = false;
				bool maxSpec = false;
				bool pastComma = false;

				while (list[k]->id() != RGX_RBRACE)
				{
					if (list[k]->id() == RGX_COMMA)
					{
						if (pastComma) return RGX_BAD_COUNT_FORM;
						pastComma = true;
					}
					else if (pastComma) //calc max
					{
						maxSpec = true;

						max *= 10;
						max += list[k]->beg() - '0';
					}
					else //calc min
					{
						minSpec = true;

						min *= 10;
						min += list[k]->beg() - '0';
					}

					k++;
				}

				if (minSpec && !pastComma)
				{
					max = min;
				}
				else if (!maxSpec)
				{
					max = -1;
				}

				list[i]->setID(RGX_COUNT);
				list[i]->setCountRange(min,max);

				for (size_t j=i+1; j<=k; j++) delete list[j];
				list.remove(i+1,k-i);
				madeChange = true;
			}
			else if (rgxmq(list,i,error)) // *? or +? or {..}?
			{
				if (!error)
				{
					list[i-1]->setGreedy(false);
					delete list[i];
					list.remove(i--);
					madeChange = true;
				}
			}
			else if (rgxmcc(list,i,error))//* or + or {..}
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
					else //if (list[i]->id() == RGX_COUNT)
					{
						list[i-1]->setCountRange(list[i]->min(), list[i]->max());
					}

					delete list[i];
					list.remove(i--);
				}
			}
			else if (rgxmpf(list,i, error)) //positive flag (?.)
			{
				if (!error)
				{
					list[i+2]->setID(RGX_POS_FLAG);
					delete list[i];
					delete list[i+1];
					delete list[i+3];
					list.remove(i+3);
					list.remove(i, 2);
					madeChange = true;
				}
			}
			else if (rgxmnf(list,i,error)) //negative flag (?-.)
			{
				if (!error)
				{
					list[i+3]->setID(RGX_NEG_FLAG);
					delete list[i];
					delete list[i+1];
					delete list[i+2];
					delete list[i+4];
					list.remove(i+4);
					list.remove(i, 3);
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
						list[i]->addChild(list[k]);
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
							list[i]->addChild(node);
							node = new rgxll(RGX_AND_LIST);
						}
						else
						{
							node->addChild(list[k]);
						}
						k++;
					}
					delete list[k];
					list.remove(i+1,k-i);

					if (list[i]->children.length())
					{
						list[i]->addChild(node);
					}
					else
					{
						delete list[i];
						list[i] = node;
					}

					madeChange = true;
				}
			}
			else if (rgxmpla(list,i,error))//(?=..)
			{
				if (!error)
				{
					list[i]->setID(RGX_POS_LOOKAHEAD);
					size_t k = i + 3;

					while (list[k]->id() != RGX_RPAREN)
					{
						list[i]->addChild(list[k]);
						k++;
					}

					delete list[i+1];
					delete list[i+2];
					delete list[k];
					list.remove(i+1,k-i);
					madeChange = true;
				}
			}
			else if (rgxmnla(list,i,error))//(?!..)
			{
				if (!error)
				{
					list[i]->setID(RGX_NEG_LOOKAHEAD);
					size_t k = i + 3;

					while (list[k]->id() != RGX_RPAREN)
					{
						list[i]->addChild(list[k]);
						k++;
					}

					delete list[i+1];
					delete list[i+2];
					delete list[k];
					list.remove(i+1,k-i);
					madeChange = true;
				}
			}
			else if (rgxmnlb(list,i,error))//(?<!..)
			{
				if (!error)
				{
					list[i]->setID(RGX_NEG_LOOKBEHIND);
					size_t k = i + 4;

					while (list[k]->id() != RGX_RPAREN)
					{
						list[i]->addChild(list[k]);
						k++;
					}

					delete list[i+1];
					delete list[i+2];
					delete list[i+3];
					delete list[k];
					list.remove(i+1,k-i);
					madeChange = true;
				}
			}
			else if (rgxmplb(list,i,error))//(?<..)
			{
				if (!error)
				{
					list[i]->setID(RGX_POS_LOOKBEHIND);
					size_t k = i + 3;

					while (list[k]->id() != RGX_RPAREN)
					{
						list[i]->addChild(list[k]);
						k++;
					}

					delete list[i+1];
					delete list[i+2];
					delete list[k];
					list.remove(i+1,k-i);
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

	if (!error && rgxmali(list,0,error))
	{
		if (list.length() > 1)
		{
			rgxll* node = new rgxll(RGX_AND_LIST);
			size_t i = 0;
			while (list.isValid(i))
			{
				if (list[i]->id() == RGX_COLUMN)
				{
					delete list[i];
					if (!root) root = new rgxll(RGX_OR_LIST);
					root->addChild(node);
					node = new rgxll(RGX_AND_LIST);
				}
				else
				{
					node->addChild(list[i]);
				}
				i++;
			}

			if (root)
			{
				root->addChild(node);
			}
			else
			{
				root = node;
			}
		}
		else if (list.length())
		{
			root = list[0];
		}
		else error = RGX_LEX_FAIL;
	}

	if (error)
	{
		root = 0;
		for (size_t i=0; i<list.length(); i++)
			delete list[i];
	}
	return error;
}
