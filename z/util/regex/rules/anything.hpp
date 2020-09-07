#pragma once
#include "rule.hpp"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			/**
			 * \brief Regex rule for matching any character.
			 */
			class anything : public rule
			{
			public:
				/**
				 * \brief Full constructor with newline option.
				 * \param newline Whether to match newline groups (\\n, \\r\\n, or just \\r)
				 * \param min The minimum number of times this rule must match.
				 * \param max The maximum number of times this rule can match.
				 * \param greedy Whether this rule should consume input greedily.
				 */
				anything(bool newline=false, int min=1, int max=1, bool greedy=true) noexcept;

				bool match(core::inputStream& stream) const noexcept;
				bool base() const noexcept;

				///If true, this rule also matches newlines. Otherwise matches anything but newlines.
				bool newline;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0) noexcept;
#			endif
			};
		}
	}
}
