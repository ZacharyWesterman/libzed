#include "lookahead.h"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			lookahead::lookahead(bool negate, size_t min, size_t max, bool greedy):
				compound(min,max,greedy), negate(negate){}

			bool lookahead::match(core::inputStream& stream) const
			{
				auto pos = stream.tell();
				size_t i = 0;
				for (auto& child : children)
				{
					if (!matchMin(child, stream))
					{
						stream.seek(pos);
						return negate;
					}

					if (child->greedy || (i >= children.length()))
					{
						for (size_t k=(child->min); k<(child->max); ++k)
						{
							if (!matchRule(child, stream)) continue;
						}
					}
					else //if not greedy, only consume until the next node matches.
					{
						auto lastPos = stream.tell();
						auto sibling = children[i+1];
						if (matchMin(sibling, stream))
						{
							stream.seek(lastPos);
							continue;
						}

						for (size_t k=(child->min); k<(child->max); ++k)
						{
							if (!matchRule(child, stream)) break;
							lastPos = stream.tell();
							if (matchMin(sibling, stream))
							{
								stream.seek(lastPos);
								break;
							}
						}
					}

					++i;
				}

				stream.seek(pos);
				return !negate;
			}

#			ifdef DEBUG
			void lookahead::print(core::outputStream& stream, int level)
			{
				if (negate)
					(zpath().padLeft(" ",(level)<<1)+"?!"+meta()).writeln(stream);
				else
					(zpath().padLeft(" ",(level)<<1)+"?="+meta()).writeln(stream);

				for (auto& child : children)
				{
					child->print(stream,level+1);
				}
			}
#			endif
		}
	}
}
