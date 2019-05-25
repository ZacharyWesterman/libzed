#pragma once
#include "compound.h"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			/**
			 * \brief Regex rule for matching a lookbehind pattern.
			 *
			 * Note that lookbehind patterns must have a constant width.
			 */
			class lookbehind : public compound
			{
			public:
				/**
				 * \brief Full constructor with negate option.
				 * \param negate Whether to perform an inverse match.
				 * \param min The minimum number of times this rule must match.
				 * \param max The maximum number of times this rule can match.
				 * \param greedy Whether this rule should consume input greedily.
				 */
				lookbehind(bool negate=false, size_t min=1, size_t max=1, bool greedy=true);
				bool match(core::inputStream& stream) const;

				///If true, negates the result of lookbehind matching.
				bool negate;

				///The number of characters in the lookbehind pattern.
				size_t width;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0);
#			endif
			};
		}
	}
}
