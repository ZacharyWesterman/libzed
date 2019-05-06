#include "rgxlex.h"

#include "rules/andlist.h"
#include "rules/boundary.h"
#include "rules/character.h"
#include "rules/compound.h"
#include "rules/lookahead.h"
#include "rules/orlist.h"
#include "rules/space.h"
#include "rules/word.h"

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
			SPACE=2,
		};

		rgxerr rgxlex(const core::array<rgxss>& input, rgx::compound** nodeOut, size_t& position, int inType, int flags)
		{
			switch (inType)
			{
				case INOR:
					*nodeOut = new rgx::orlist;
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
