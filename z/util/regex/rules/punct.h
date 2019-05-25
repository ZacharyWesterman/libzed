#pragma once
#include "rule.h"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			/**
			 * \brief Regex rule for matching punctuation characters.
			 *
			 * Punctuation is defined as any of the following characters:
			 *
			 * !\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~
			 */
			class punct : public rule
			{
			public:
				/**
				 * \brief Full constructor with negate option.
				 * \param negate Whether to perform an inverse match.
				 * \param min The minimum number of times this rule must match.
				 * \param max The maximum number of times this rule can match.
				 * \param greedy Whether this rule should consume input greedily.
				 */
				punct(bool negate=false, size_t min=1, size_t max=1, bool greedy=true);
				bool match(uint32_t current) const;

				///If true, only matches non-punctuation characters.
				bool negate;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0);
#			endif
			};
		}
	}
}
