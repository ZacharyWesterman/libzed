#include "rgxlex.hpp"
#include "rgxid.hpp"
#include "rgxflag.hpp"

#include "rgx.hpp" //all regex rules

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

		//get char width of compound node, or -1 if it is not a constant width.
		static int getLBwidth(const rgx::compound* node) noexcept
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
					setw = true;
					width = neww;
				}
				else
				{
					width += neww;
				}
			}

			return width;
		}

		static rgx::compound* makeParent(int inType, int flags) noexcept
		{
			switch (inType)
			{
				case INOR:
					return new rgx::orlist;
				case INAHEAD:
					return new rgx::lookahead(flags & NEGATE);
				case INBEHIND:
					return new rgx::lookbehind(flags & NEGATE);
				default:
					return new rgx::andlist;
			}
		}

		static rgxerr getFlags(const core::array<rgxss>& input, int& position, int& flags, bool negate) noexcept
		{
			rgxerr err = negate ? RGX_BAD_NEG_FLAG : RGX_BAD_POS_FLAG;

			if (position >= input.length()) return err;
			while (position < input.length())
			{
				auto id = input[position].id();

				if (id == RGX_RPAREN) break;
				else if (id != RGX_SYMBOL) return err;

				switch (input[position].symbol())
				{
					case 'i':
						flags = negate ? (flags & ~INSENSITIVE) : (flags | INSENSITIVE);
						break;
					case 's':
						flags = negate ? (flags & ~NEWLINE) : (flags | NEWLINE);
						break;
					case 'n':
						flags = negate ? (flags & ~ENDLINE) : (flags | ENDLINE);
						break;
					default:
						return err;
				}

				++position;
			}

			return RGX_NO_ERROR;
		}

		static rgxerr setCount(rgx::compound** nodeOut, int min, int max) noexcept
		{
			auto len = (*nodeOut)->children.length();
			if (!len) return RGX_BAD_COUNT_LOC;
			auto child = (*nodeOut)->children[len-1];
			child->min = min;
			child->max = max;
			return RGX_NO_ERROR;
		}

		static rgxerr setGreed(rgx::compound** nodeOut, bool greedy) noexcept
		{
			auto len = (*nodeOut)->children.length();
			if (!len) return RGX_BAD_GREED_LOC;
			auto child = (*nodeOut)->children[len-1];
			child->greedy = greedy;
			return RGX_NO_ERROR;
		}

		rgx::compound* rgxlex(const core::array<rgxss>& input, rgxerr& err, int& position, int inType, int flags) noexcept
		{
			auto nodeOut = makeParent(inType, flags);

			rgx::compound* andOptions = NULL;
			err = RGX_NO_ERROR;
			bool done = false;

			while ((position < input.length()) && !done)
			{
				auto id = input[position].id();
				auto chr = input[position].symbol();

				rgx::rule* child = NULL;

				switch (id)
				{
					case RGX_SYMBOL:
						if (input.isValid(position+1) && (input[position+1].id() == RGX_DASH))
						{
							position+=2;
							child = new rgx::range(chr, input[position].symbol(), flags & INSENSITIVE);
						}
						else
						{
							child = new rgx::character(chr, flags & INSENSITIVE);
						}
						break;
					case RGX_BREAK:
						child = new rgx::boundary(false);
						break;
					case RGX_NOT_BREAK:
						child = new rgx::boundary(true);
						break;
					case RGX_WORD:
						child = new rgx::word(false);
						break;
					case RGX_NOT_WORD:
						child = new rgx::word(true);
						break;
					case RGX_ALPHA:
						child = new rgx::alpha(false);
						break;
					case RGX_NOT_ALPHA:
						child = new rgx::alpha(true);
						break;
					case RGX_ALNUM:
						child = new rgx::alnum(false);
						break;
					case RGX_NOT_ALNUM:
						child = new rgx::alnum(true);
						break;
					case RGX_NEWLINE:
						child = new rgx::newline;
						break;
					case RGX_PUNCT:
						child = new rgx::punct(false);
						break;
					case RGX_NOT_PUNCT:
						child = new rgx::punct(true);
						break;
					case RGX_SPACE:
						child = new rgx::space(false);
						break;
					case RGX_NOT_SPACE:
						child = new rgx::space(true);
						break;
					case RGX_LPAREN:
						++position;
						child = rgxlex(input, err, position, INAND, flags);
						break;
					case RGX_RPAREN:
						done = true;
						continue;
					case RGX_COLUMN:
						if (!andOptions) andOptions = new rgx::orlist;
						andOptions->children.add(nodeOut);
						nodeOut = makeParent(inType, flags);
						break;
					case RGX_LBRACKET:
						++position;
						child = rgxlex(input, err, position, INOR, flags);
						break;
					case RGX_RBRACKET:
						done = true;
						continue;
					case RGX_NOT:
						((rgx::orlist*)nodeOut)->negate = true;
						break;
					case RGX_POS_LOOKAHEAD:
						++position;
						child = rgxlex(input, err, position, INAHEAD, flags);
						break;
					case RGX_NEG_LOOKAHEAD:
						++position;
						child = rgxlex(input, err, position, INAHEAD, flags | NEGATE);
						break;
					case RGX_POS_LOOKBEHIND:
						++position;
						child = rgxlex(input, err, position, INBEHIND, flags);
						if (!err)
						{
							int width = getLBwidth((rgx::compound*)child);
							if (width < 0) err = RGX_BAD_LOOKBEHIND;
						}
						break;
					case RGX_NEG_LOOKBEHIND:
						++position;
						child = rgxlex(input, err, position, INBEHIND, flags | NEGATE);
						if (!err)
						{
							int width = getLBwidth((rgx::compound*)child);
							if (width < 0) err = RGX_BAD_LOOKBEHIND;
						}
						break;
					case RGX_PERIOD:
						child = new rgx::anything(flags & NEWLINE);
						break;
					case RGX_POS_FLAG:
						++position;
						err = getFlags(input, position, flags, false);
						break;
					case RGX_NEG_FLAG:
						++position;
						err = getFlags(input, position, flags, true);
						break;
					case RGX_DIGIT:
						child = new rgx::digit;
						break;
					case RGX_PLUS:
						err = setCount(&nodeOut,1,-1);
						break;
					case RGX_ASTERISK:
						err = setCount(&nodeOut,0,-1);
						break;
					case RGX_LBRACE:
						id = input[++position].id();
						chr = input[position].symbol();
						if (input[++position].id() == RGX_RBRACE)//{x}
						{
							err = setCount(&nodeOut,chr,chr);
						}
						else if (id == RGX_COUNT)//{,x} {x,} {x,x} -> all come in as {x,} or {x,x}
						{
							id = input[++position].id();
							if (id == RGX_RBRACE)
								err = setCount(&nodeOut,chr,-1);
							else if ((id == RGX_COUNT) && (input[position+1].id() == RGX_RBRACE))
								err = setCount(&nodeOut,chr,input[position++].symbol());
							else
								err = RGX_BAD_COUNT_FORM;
						}
						else
						{
							err = RGX_BAD_COUNT_FORM;
						}
						break;
					case RGX_BEGIN:
						child = new rgx::begin;
						break;
					case RGX_END:
						child = new rgx::end(flags & ENDLINE);
						break;
					case RGX_QUESTION:
						err = setCount(&nodeOut,0,1);
						break;
					case RGX_GREEDY:
						err = setGreed(&nodeOut,false);
						break;
					default:
						err = RGX_ERROR;
				}

				if (err)
				{
					if (child) delete child;
					break;
				}

				if (child) nodeOut->children.add(child);
				++position;
			}

			if (andOptions)
			{
				andOptions->children.add(nodeOut);
				nodeOut = andOptions;
			}

			//If possible, trim child nodes to reduce regex size
			bool canReduce = (inType == INAND) || ((inType == INOR) && !((rgx::orlist*)nodeOut)->negate);
			if (canReduce && (nodeOut->children.length() == 1) && (position < input.length()))
			{
				auto temp = nodeOut->children[0];
				nodeOut->children.clear();
				delete nodeOut;
				nodeOut = (rgx::compound*)temp;
			}

			return nodeOut;
		}
	}
}
