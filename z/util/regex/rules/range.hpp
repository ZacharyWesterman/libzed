#pragma once
#include "rule.hpp"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			/**
			* \brief Regex rule for matching a range of characters.
			*/
			class range : public rule
			{
			public:
				/**
				* \brief Full constructor with character range, inclusive.
				* \param begin The lowest character value to match.
				* \param end The highest character value to match.
				* \param insensitive Whether character matching should be case-insensitive.
				* \param min The minimum number of times this rule must match.
				* \param max The maximum number of times this rule can match.
				* \param greedy Whether this rule should consume input greedily.
				*/
				range(uint32_t begin, uint32_t end, bool insensitive=false, int min=1, int max=1, bool greedy=true) noexcept;

				bool match(uint32_t current) const noexcept;

				///The lowest character value to match.
				uint32_t begin;

				///The highest character value to match.
				uint32_t end;

				///Whether character matching should be case-insensitive.
				bool insensitive;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0) noexcept;
#			endif
			};
		}
	}
}
