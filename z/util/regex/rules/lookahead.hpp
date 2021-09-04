#pragma once
#include "compound.hpp"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			/**
			* \brief Regex rule for matching a lookahead pattern.
			*/
			class lookahead : public compound
			{
			public:
				/**
				* \brief Full constructor with negate option.
				* \param negate Whether to perform an inverse match.
				* \param min The minimum number of times this rule must match.
				* \param max The maximum number of times this rule can match.
				* \param greedy Whether this rule should consume input greedily.
				*/
				lookahead(bool negate=false, int min=1, int max=1, bool greedy=true) noexcept;
				bool match(core::inputStream& stream) const noexcept;

				///If true, negates the result of lookahead matching.
				bool negate;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0) noexcept;
#			endif

			};
		}
	}
}
