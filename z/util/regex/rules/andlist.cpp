#include "andlist.hpp"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			bool andlist::match(core::inputStream& stream) const
			{
				auto pos = stream.tell();
				int i = 0;
				for (auto& child : children)
				{
					if (!matchMin(child, stream))
					{
						stream.seek(pos);
						return false;
					}

					if (child->greedy || (i+1 >= children.length()))
					{
						for (int k=(child->min); k<(child->max); ++k)
						{
							auto lastPos = stream.tell();
							if (!matchRule(child, stream))
							{
								if (!stream.empty()) stream.seek(lastPos);
								break;
							}
						}
					}
					else //if not greedy, only consume until the next node matches.
					{
						size_t lastPos;
						auto sibling = children[i+1];
						for (int k=(child->min); k<(child->max); ++k)
						{
							lastPos = stream.tell();
							if (matchMin(sibling, stream))
							{
								stream.seek(lastPos);
								break;
							}
							stream.seek(lastPos);
							if (!matchRule(child, stream))
							{
								stream.seek(lastPos);
								break;
							}
						}
					}

					++i;
				}

				return true;
			}

#			ifdef DEBUG
			void andlist::print(core::outputStream& stream, int level)
			{
				zpath s;
				s.padLeftIn(" ",(level)<<1);
				s += "()";
				(s+meta()).writeln(stream);
				for (auto& child : children)
				{
					child->print(stream,level+1);
				}
			}
#			endif
		}
	}
}
