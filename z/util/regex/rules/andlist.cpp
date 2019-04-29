#include "andlist.h"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			bool andlist::match(core::inputStream& stream) const
			{
				auto pos = stream.tell();
				size_t i = 0;
				for (auto& child : children)
				{
					if (!matchMin(child, stream))
					{
						stream.seek(pos);
						return false;
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

				return true;
			}
		}
	}
}
