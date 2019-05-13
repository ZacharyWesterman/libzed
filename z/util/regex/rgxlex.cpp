#include "rgxlex.h"

#include "rules/andlist.h"
#include "rules/boundary.h"
#include "rules/character.h"
#include "rules/compound.h"
#include "rules/lookahead.h"
#include "rules/lookbehind.h"
#include "rules/orlist.h"
#include "rules/space.h"
#include "rules/word.h"
#include "rules/anything.h"

#include "rgxid.h"

namespace z
{
	namespace util
	{
		enum
		{
			INAND,
			INOR,
			INAHEAD,
			INBEHIND,
		};

		enum
		{
			CASEI=1,
			NEWLINE=2,
			NEGATE=4,
		};

		//get char width of compound node, or -1 if it is not a constant width.
		static int getLBwidth(const rgx::compound* node)
		{
			bool setw = false;
			int width = 0;
			bool isOr = node->isOr();

			for (auto& child : node->children)
			{
				if (child->min != child->max) return -1;

				int neww;
				if (child->parent())
					neww = getLBwidth((rgx::compound*)child);
				else
					neww = (int)child->min;

				if (neww < 0) return neww;

				if (isOr)
				{
					if (setw && (width != neww)) return -1;
					return -1;
					// setw = true;
					// width = neww;
				}
				else
				{
					width += neww;
				}
			}

			return width;
		}

		rgxerr rgxlex(const core::array<rgxss>& input, rgx::compound** nodeOut, size_t& position, int inType, int flags)
		{
			switch (inType)
			{
				case INOR:
					*nodeOut = new rgx::orlist;
					break;
				case INAHEAD:
					*nodeOut = new rgx::lookahead(flags & NEGATE);
					break;
				case INBEHIND:
					*nodeOut = new rgx::lookbehind(flags & NEGATE);
					break;
				default:
					*nodeOut = new rgx::andlist;
			}

			rgx::compound* andOptions = NULL;
			rgxerr err = RGX_NO_ERROR;
			bool done = false;

			while ((position < input.length()) && !done)
			{
				auto id = input[position].id();
				auto chr = input[position].symbol();

				rgx::rule* child = NULL;

				switch (id)
				{
					case RGX_SYMBOL:
						child = new rgx::character(chr);
						break;
					case RGX_LPAREN:
						++position;
						err = rgxlex(input,(rgx::compound**)&child, position, INAND, flags);
						break;
					case RGX_RPAREN:
						done = true;
						continue;
					case RGX_COLUMN:
						if (!andOptions) andOptions = new rgx::orlist;
						andOptions->children.add(*nodeOut);
						*nodeOut = new rgx::andlist;
						break;
					case RGX_LBRACKET:
						++position;
						err = rgxlex(input,(rgx::compound**)&child, position, INOR, flags);
						break;
					case RGX_RBRACKET:
						done = true;
						continue;
					case RGX_NOT:
						((rgx::orlist*)*nodeOut)->negate = true;
						break;
					case RGX_POS_LOOKAHEAD:
						++position;
						err = rgxlex(input,(rgx::compound**)&child, position, INAHEAD, flags);
						break;
					case RGX_NEG_LOOKAHEAD:
						++position;
						err = rgxlex(input,(rgx::compound**)&child, position, INAHEAD, flags | NEGATE);
						break;
					case RGX_POS_LOOKBEHIND:
						++position;
						err = rgxlex(input,(rgx::compound**)&child, position, INBEHIND, flags);
						if (!err)
						{
							int width = getLBwidth((rgx::compound*)child);
							if (width < 0) err = RGX_BAD_LOOKBEHIND;
						}
						break;
					case RGX_NEG_LOOKBEHIND:
						++position;
						err = rgxlex(input,(rgx::compound**)&child, position, INBEHIND, flags | NEGATE);
						if (!err)
						{
							int width = getLBwidth((rgx::compound*)child);
							if (width < 0) err = RGX_BAD_LOOKBEHIND;
						}
						break;
					case RGX_PERIOD:
						child = new rgx::anything(flags & NEWLINE);
						break;
					default:
						err = RGX_ERROR;
				}

				if (err)
				{
					if (child) delete child;
					break;
				}

				if (child) (*nodeOut)->children.add(child);
				++position;
			}

			if (andOptions)
			{
				andOptions->children.add(*nodeOut);
				*nodeOut = andOptions;
			}
			return err;
		}
	}
}
