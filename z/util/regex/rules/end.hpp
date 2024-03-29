#pragma once
#include "rule.hpp"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			/**
			* \brief Regex rule for matching the end of the stream or a newline.
			*/
			class end : public rule
			{
			public:
				/**
				* \brief Full constructor with newline option.
				* \param newline Whether to match newline groups (\\n, \\r\\n, or just \\r)
				* \param min The minimum number of times this rule must match.
				* \param max The maximum number of times this rule can match.
				* \param greedy Whether this rule should consume input greedily.
				*/
				end(bool newline=false, int min=1, int max=1, bool greedy=true) noexcept;

				bool match(core::inputStream& stream) const noexcept;
				bool base() const noexcept;

				///If true, this rule also matches newlines. Otherwise only matches the end of the stream.
				bool newline;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0) noexcept;
#			endif
			};
		}
	}
}
