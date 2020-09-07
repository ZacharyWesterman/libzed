#include "lookahead.hpp"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			lookahead::lookahead(bool negate, int min, int max, bool greedy) noexcept :
				compound(min,max,greedy), negate(negate){}

			bool lookahead::match(core::inputStream& stream) const noexcept
			{
				if (stream.empty()) return negate;
				auto pos = stream.tell();
				int i = 0;
				for (auto& child : children)
				{
					if (!matchMin(child, stream))
					{
						stream.seek(pos);
						return false;
					}

					if (child->greedy || (i >= children.length()))
					{
						for (int k=(child->min); k<(child->max); ++k)
						{
							auto lastPos = stream.tell();
							if (!matchRule(child, stream))
							{
								stream.seek(lastPos);
								break;
							}
						}
					}
					else //if not greedy, only consume until the next node matches.
					{
						auto lastPos = stream.tell();
						auto sibling = children[i+1];
						if (matchMin(sibling, stream))
						{
							stream.seek(lastPos);
							++i;
							continue;
						}

						for (int k=(child->min); k<(child->max); ++k)
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
			void lookahead::print(core::outputStream& stream, int level) noexcept
			{
				if (negate)
					(zpath().padLeftIn(" ",(level)<<1)+"?!"+meta()).writeln(stream);
				else
					(zpath().padLeftIn(" ",(level)<<1)+"?="+meta()).writeln(stream);

				for (auto& child : children)
				{
					child->print(stream,level+1);
				}
			}
#			endif
		}
	}
}
