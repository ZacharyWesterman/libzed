#pragma once
#include "rule.hpp"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			/**
			 * \brief Regex rule for matching a specific character.
			 */
			class character : public rule
			{
			public:
				/**
				 * \brief Full constructor with character value.
				 * \param value The character to match.
				 * \param insensitive Whether character matching should be case-insensitive.
				 * \param min The minimum number of times this rule must match.
				 * \param max The maximum number of times this rule can match.
				 * \param greedy Whether this rule should consume input greedily.
				 */
				character(uint32_t value, bool insensitive=false, int min=1, int max=1, bool greedy=true);

				bool match(uint32_t current) const;

				///The character we want to match.
				uint32_t value;

				///Whether character matching should be case-insensitive.
				bool insensitive;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0);
#			endif
			};
		}
	}
}
