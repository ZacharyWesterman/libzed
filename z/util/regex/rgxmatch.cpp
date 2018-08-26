#include <cstdint>
#include <cstddef>

#include "rgxmatch.h"

#include <z/core/charFunctions.h>

namespace z
{
	namespace util
	{

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

			matcher->pushFlags();
			while (result && matcher->node->sibling())
			{

				matcher->node = matcher->node->sibling();

				if (matcher->node->min() == matcher->node->max())
				{
					result = rgxmatch(matcher);
					break;
				}
				else
				{
					result = rgxmatchmin(matcher);
				}
			}
			matcher->popFlags();

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
						if (!(matcher->node->greedy()))
						{
							return rgxmatchsuccess(matcher);
						}

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
			uint32_t beg = matcher->node->beg();
			uint32_t end = matcher->node->end();

			bool casei = matcher->getFlag(RGX_FLAG_CASEI);

			if (beg == end)
			{
				if (casei)
				{
					ch = core::toUpper(ch);
					beg = core::toUpper(beg);
				}

				return (ch == beg);
			}
			else
			{
				if ((ch < beg) || (ch > end))
				{
					ch = core::toUpper(ch);
					beg = core::toUpper(beg);
					end = core::toUpper(end);

					return ((ch >= beg) && (ch <= end));
				}

				return true;
			}
		}

		bool rgxmatchanything(rgxmatcher* matcher)
		{
			if (matcher->stream->empty()) return false;

			uint32_t ch = matcher->stream->getChar(matcher->format);

			if (matcher->getFlag(RGX_FLAG_NEWLINE)) return true;

			return (ch != '\n');
		}

		bool rgxmatchword(rgxmatcher* matcher, bool negate)
		{
			uint32_t ch = matcher->stream->getChar(matcher->format);

			if (core::isAlphaNumeric(ch) || (ch == '_'))
				return !negate;
			else
				return negate;
		}

		bool rgxmatchspace(rgxmatcher* matcher, bool negate)
		{
			uint32_t ch = matcher->stream->getChar(matcher->format);

			if (core::isWhiteSpace(ch))
				return !negate;
			else
				return negate;
		}

		bool rgxmatchalpha(rgxmatcher* matcher, bool negate)
		{
			uint32_t ch = matcher->stream->getChar(matcher->format);

			if (core::isAlpha(ch))
				return !negate;
			else
				return negate;
		}

		bool rgxmatchdigit(rgxmatcher* matcher, bool negate)
		{
			uint32_t ch = matcher->stream->getChar(matcher->format);

			if (core::isNumeric(ch))
				return !negate;
			else
				return negate;
		}

		bool rgxmatchalnum(rgxmatcher* matcher, bool negate)
		{
			uint32_t ch = matcher->stream->getChar(matcher->format);

			if (core::isAlphaNumeric(ch))
				return !negate;
			else
				return negate;
		}

		bool rgxmatchbreak(rgxmatcher* matcher, bool negate)
		{
			size_t streamIndex = matcher->stream->tell();

			//match if at either end of the stream
			if (matcher->stream->empty()) return !negate;
			if (!streamIndex) return !negate;

			//move back 1 character
			matcher->stream->seek(streamIndex - matcher->charsToOffset(1));

			uint32_t ch1 = matcher->stream->getChar(matcher->format);
			uint32_t ch2 = matcher->stream->getChar(matcher->format);

			matcher->stream->seek(streamIndex);

			//if between two word-characters, we're not at a break
			if (core::isAlphaNumeric(ch1) || (ch1 == '_'))
			{
				if (core::isAlphaNumeric(ch2) || (ch2 == '_'))
					return negate;
			}

			return !negate;
		}

		bool rgxmatchpunct(rgxmatcher* matcher, bool negate)
		{
			uint32_t ch = matcher->stream->getChar(matcher->format);

			if (!ch) return negate;

			const uint32_t charList[] =
			{
				'[', '!', '"', '#', '$', '%',
				'&', '\'', '(', ')', '*', '+',
				',', '-', '.', '/', ':', ';',
				'<', '=', '>', '?', '@', '[',
				'\\', ']', '^', '_', '`', '{',
				'|', '}', '~', ']',
				0 // end here
			};

			size_t index = 0;
			while (charList[index])
			{
				if (ch == charList[index])
				{
					return !negate;
				}

				index++;
			}

			return negate;
		}

		//never consume input
		bool rgxmatchlookahead(rgxmatcher* matcher, bool negate)
		{
			size_t streamIndex = matcher->stream->tell();

			size_t children = matcher->node->children.length();
			for (size_t i=0; i<children; i++)
			{
				rgxll* node = matcher->node;
				matcher->node = node->children[i];

				if (!rgxmatchonce(matcher)) //an item didn't match, fail.
				{
					matcher->node = node;
					matcher->stream->seek(streamIndex);
					return negate;
				}

				matcher->node = node;
			}

			//no items that didn't match, success
			matcher->stream->seek(streamIndex);
			return !negate;
		}

		//never consume input
		bool rgxmatchlookbehind(rgxmatcher* matcher, bool negate)
		{
			size_t streamIndex = matcher->stream->tell();

			size_t charSz = matcher->charsToOffset(1);
			size_t index = streamIndex;

			size_t children = matcher->node->children.length();
			for (size_t i=0; i<children; i++)
			{
				rgxll* node = matcher->node;
				matcher->node = node->children[children-i-1];

				index -= charSz;
				matcher->stream->seek(index);
				if (!rgxmatchonce(matcher)) //an item didn't match, fail.
				{
					matcher->node = node;
					matcher->stream->seek(streamIndex);
					return negate;
				}

				matcher->node = node;
			}

			//no items that didn't match, success
			matcher->stream->seek(streamIndex);
			return !negate;
		}

		bool rgxsetflag(rgxmatcher* matcher, bool state)
		{
			uint32_t ch = matcher->node->beg();

			switch(ch)
			{
				case 'i':
					matcher->setFlag(RGX_FLAG_CASEI, state);
					return true;

				case 's':
					matcher->setFlag(RGX_FLAG_NEWLINE, state);
					return true;

				default:
					return false;
			}
		}

		bool rgxmatchonce(rgxmatcher* matcher)
		{
			if (!(matcher && matcher->node)) return false;

			bool negate = false;
			bool result = false;

			switch (matcher->node->id())
			{
				case RGX_OR_LIST:
					result = rgxmatchorlist(matcher);
					break;

				case RGX_AND_LIST:
					if (matcher->node->parent())
					{
						matcher->pushFlags();
						result = rgxmatchandlist(matcher);
						matcher->popFlags();
					}
					else
					{
						result = rgxmatchandlist(matcher);
					}
					break;

				case RGX_SYMBOL:
					result = rgxmatchsymbol(matcher);
					break;

				case RGX_NOT_WORD:
					negate = true;
				case RGX_WORD:
					result = rgxmatchword(matcher, negate);
					break;

				case RGX_NEG_FLAG:
					negate = true;
				case RGX_POS_FLAG:
					result = rgxsetflag(matcher, !negate);
					break;

				case RGX_PERIOD:
					result = rgxmatchanything(matcher);
					break;

				case RGX_NOT_SPACE:
					negate = true;
				case RGX_SPACE:
					result = rgxmatchspace(matcher, negate);
					break;

				case RGX_NOT_ALPHA:
					negate = true;
				case RGX_ALPHA:
					result = rgxmatchalpha(matcher, negate);
					break;

				case RGX_NOT_DIGIT:
					negate = true;
				case RGX_DIGIT:
					result = rgxmatchdigit(matcher, negate);
					break;

				case RGX_NOT_ALNUM:
					negate = true;
				case RGX_ALNUM:
					result = rgxmatchalnum(matcher, negate);
					break;

				case RGX_NOT_BREAK:
					negate = true;
				case RGX_BREAK:
					result = rgxmatchbreak(matcher, negate);
					break;

				case RGX_NOT_PUNCT:
					negate = true;
				case RGX_PUNCT:
					result = rgxmatchpunct(matcher, negate);
					break;

				case RGX_BEGIN:
					result = !(matcher->stream->tell());
					break;

				case RGX_END:
					result = !(matcher->stream->empty());
					break;

				case RGX_NEG_LOOKAHEAD:
					negate = true;
				case RGX_POS_LOOKAHEAD:
					result = rgxmatchlookahead(matcher, negate);
					break;

				case RGX_NEG_LOOKBEHIND:
					negate = true;
				case RGX_POS_LOOKBEHIND:
					result = rgxmatchlookbehind(matcher, negate);
					break;

				default:
					matcher->fail = true;
			}

			return result;
		}
	}
}
