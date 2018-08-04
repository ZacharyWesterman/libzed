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

bool rgxmatchonce(rgxmatcher* matcher);

bool rgxmatch(rgxmatcher* matcher)
{
	if (matcher && !matcher->fail)
	{
		if (matcher->stream->bad())
		{
			matcher->fail = true;
			return false;
		}

		return rgxmatchonce(matcher);
		return true;
	}

	return false;
}

bool rgxmatchonce(rgxmatcher* matcher)
{
	size_t streamIndex = matcher->stream->tell();
	uint32_t ch;

	switch (matcher->node->id())
	{
		case RGX_OR_LIST:
			if (matcher->node->children[0]->id() == RGX_NOT)
			{
				// for (size_t i=1; i<(matcher->node->children.length()); i++)
				// {
				// 	if (matcher->fail) return false;
				// 	matcher->node = matcher->node->children[i];
				// 	if (rgxmatch(matcher)) // negated OR fails on any match
				// 	{
				// 		matcher->stream->seek(streamIndex);
				// 		matcher->node = matcher->node->parent();
				// 		return false;
				// 	}
				// }
				matcher->stream->seek(streamIndex+1); //consume the character we didn't match
				// matcher->node = matcher->node->parent();
				// return true;
			}
			else
			{
				for (size_t i=0; i<(matcher->node->children.length()); i++)
				{
					if (matcher->fail) return false;
					matcher->node = matcher->node->children[i];
					if (rgxmatch(matcher)) // positive OR gives success on any match
					{
						matcher->node = matcher->node->parent();
						return true;
					}
				}

				matcher->stream->seek(streamIndex);
				matcher->node = matcher->node->parent();
				return false;
			}
			break;

		case RGX_AND_LIST:
			for (size_t i=0; i<(matcher->node->children.length()); i++)
			{
				if (matcher->fail) return false;

				matcher->node = matcher->node->children[i];
				if (!rgxmatch(matcher)) // AND fails when any child doesn't match
				{
					matcher->stream->seek(streamIndex);
					matcher->node = matcher->node->parent();
					return false;
				}
			}
			break;

		case RGX_SYMBOL:
			ch = matcher->stream->getChar(matcher->format);
			std::cout << "CH:" << (char)ch << std::endl;
			if ((ch < matcher->node->beg()) || (ch > matcher->node->end()))
			{
				//not within character range
				matcher->stream->seek(streamIndex);
				matcher->node = matcher->node->parent();
				return false;
			}
			break;

		default:
			matcher->fail = true;
			return false;
	}

	//Should only get here on successful match
	matcher->node = matcher->node->parent();
	return true;
}
