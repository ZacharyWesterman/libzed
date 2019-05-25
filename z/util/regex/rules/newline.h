#pragma once
#include "rule.h"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			/**
			 * \brief Regex rule for matching newlines (\\n, \\r\\n, or just \\r).
			 */
			class newline : public rule
			{
			public:
				bool match(core::inputStream& stream) const;
				bool base() const;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0);
#			endif
			};
		}
	}
}
