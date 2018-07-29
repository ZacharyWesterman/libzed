#pragma once

bool rgxmci(const core::array<rgxll*>& list, size_t index, rgxerr& error) // (?i)
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

bool rgxmcni(const core::array<rgxll*>& list, size_t index, rgxerr& error) // (?!i)
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
				}
			}
			else if (rgxmcni(list,i,error)) //end case-insensitive
			{
				if (!error)
				{
					list[i]->setID(RGX_NOT_CASEI);
					for (size_t k=i+1; k<(i+4); k++) delete list[k];
					list.remove(i+1, 4);
				}
			}

			if (error)
			{
				madeChange = false;
				break;
			}
		}
	}

	if (list.length())
	{
		root = new rgxll(RGX_TEST);
		for (size_t i=0; i<list.length(); i++)
			root->children.add(list[i]);
	}
	else error = RGX_LEX_FAIL;

	return error;
}
