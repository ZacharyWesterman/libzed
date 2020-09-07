#pragma once
#include "rule.hpp"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			/**
			 * \brief Regex rule for checking whether the stream is at a word boundary.
			 */
			class boundary : public rule
			{
			public:
				/**
				 * \brief Full constructor with negate option.
				 * \param negate Whether to perform an inverse match.
				 * \param min The minimum number of times this rule must match.
				 * \param max The maximum number of times this rule can match.
				 * \param greedy Whether this rule should consume input greedily.
				 */
				boundary(bool negate=false, int min=1, int max=1, bool greedy=true) noexcept;

				bool match(core::inputStream& stream) const noexcept;
				bool base() const noexcept;

				///If true, matches only if stream is NOT at a word boundary.
				bool negate;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0) noexcept;
#			endif
			};
		}
	}
}
