#include "compound.hpp"
namespace z
{
	namespace util
	{
		namespace rgx
		{
			compound::~compound() noexcept
			{
				for (auto& child : children) delete child;
			}

			bool compound::matchRule(rule* rgxRule, core::inputStream& stream) const noexcept
			{
				if (rgxRule->base())
				{
					auto ch = stream.getChar();
					if (stream.empty()) return false;
					return rgxRule->match(ch);
				}
				else
				{
					return rgxRule->match(stream);
				}
			}

			bool compound::matchMin(rule* rgxRule, core::inputStream& stream) const noexcept
			{
				for (int i=0; i<(rgxRule->min); ++i)
				{
					if (!matchRule(rgxRule, stream)) return false;
				}
				return true;
			}
		}
	}
}
