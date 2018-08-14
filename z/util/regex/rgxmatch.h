#pragma once

struct rgxmatcher
{
	core::inputStream* stream;
	rgxll* node;
	encoding format;
	bool fail;

	rgxmatcher(core::inputStream* input, rgxll* root, encoding charType)
	{
		fail = !(input && root);
		stream = input;
		node = root;
		format = charType;
	}
};

bool rgxmatchsuccess(rgxmatcher* matcher)
{
	matcher->node = matcher->node->parent();
	return true;
}

bool rgxmatchfail(rgxmatcher* matcher, size_t streamIndex)
{
	matcher->stream->seek(streamIndex);
	matcher->node = matcher->node->parent();
	return false;
}

bool rgxmatchonce(rgxmatcher* matcher);
bool rgxmatch(rgxmatcher* matcher);

bool rgxmatchmin(rgxmatcher* matcher)
{
	size_t min = matcher->node->min();

	//must at least match the minimum
	for (size_t i=0; i<min; i++)
	{
		if (!rgxmatchonce(matcher)) return false;
	}

	return true;
}

bool rgxmatchnext(rgxmatcher* matcher) //does not consume stream
{
	if (matcher->fail) return false;

	rgxll* prev = matcher->node;
	size_t streamIndex = matcher->stream->tell();

	bool result = true;

	while (result && matcher->node->sibling())
	{
		matcher->node = matcher->node->sibling();

		result = rgxmatchmin(matcher);
	}

	matcher->node = prev;
	matcher->stream->seek(streamIndex);
	return result;
}

bool rgxmatch(rgxmatcher* matcher)
{
	if (matcher && !matcher->fail)
	{
		size_t streamIndex = matcher->stream->tell();

		if (!rgxmatchmin(matcher))
		{
			return rgxmatchfail(matcher, streamIndex);
		}

		streamIndex = matcher->stream->tell();
		size_t lastMatchIndex = -1;

		size_t min = matcher->node->min();
		size_t max = matcher->node->max();

		for (size_t i=min; i<max; i++)
		{
			if (matcher->fail) return false;

			if (rgxmatchnext(matcher))
			{
				lastMatchIndex = matcher->stream->tell();
			}

			if (!rgxmatchonce(matcher))
			{
				if (lastMatchIndex < (size_t)-1)
				{
					matcher->stream->seek(lastMatchIndex);
				}
				else
				{
					matcher->stream->seek(streamIndex);
				}
				return rgxmatchsuccess(matcher);
			}

			streamIndex = matcher->stream->tell();
		}

		return rgxmatchsuccess(matcher);
	}

	return false;
}

bool rgxmatchorlist(rgxmatcher* matcher)
{
	if (matcher->node->children[0]->id() == RGX_NOT)
	{
		for (size_t i=1; i<(matcher->node->children.length()); i++)
		{
			if (matcher->fail) return false;
			matcher->node = matcher->node->children[i];
			if (rgxmatch(matcher)) // negated OR fails on any match
			{
				return false;
			}
		}
		matcher->stream->getChar(matcher->format); //consume the character we didn't match
		return true;
	}
	else
	{
		for (size_t i=0; i<(matcher->node->children.length()); i++)
		{
			if (matcher->fail) return false;
			matcher->node = matcher->node->children[i];
			if (rgxmatch(matcher)) // positive OR gives success on any match
			{
				return true;
			}
		}

		return false;
	}
}

bool rgxmatchandlist(rgxmatcher* matcher)
{
	size_t count = matcher->node->children.length();
	for (size_t i=0; i<count; i++)
	{
		if (matcher->fail) return false;

		matcher->node = matcher->node->children[i];
		if (!rgxmatch(matcher)) // AND fails when any child doesn't match
		{
			return false;
		}
	}

	return true;
}

bool rgxmatchsymbol(rgxmatcher* matcher)
{
	uint32_t ch = matcher->stream->getChar(matcher->format);
	if ((ch < matcher->node->beg()) || (ch > matcher->node->end()))
	{
		// not within character range
		return false;
	}
	return true;
}

bool rgxmatchword(rgxmatcher* matcher, bool negate)
{
	uint32_t ch = matcher->stream->getChar(matcher->format);

	if (core::isAlphaNumeric(ch) || (ch == '_'))
		return !negate;
	else
		return negate;
}

bool rgxmatchonce(rgxmatcher* matcher)
{
	if (!(matcher && matcher->node)) return false;

	bool negate = false;

	switch (matcher->node->id())
	{
		case RGX_OR_LIST:
			return rgxmatchorlist(matcher);
			break;

		case RGX_AND_LIST:
			return rgxmatchandlist(matcher);
			break;

		case RGX_SYMBOL:
			return rgxmatchsymbol(matcher);
			break;

		case RGX_NOT_WORD:
			negate = true;
		case RGX_WORD:
			return rgxmatchword(matcher, negate);
			break;

		default:
			matcher->fail = true;
			return false;
	}

	//Should never get here
}
