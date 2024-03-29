#pragma once
#include "rule.hpp"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			/**
			* \brief Regex rule for matching a digit 0-9
			*/
			class digit : public rule
			{
			public:
				bool match(uint32_t current) const noexcept;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0) noexcept;
#			endif
			};
		}
	}
}
