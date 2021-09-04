#pragma once
#include "compound.hpp"

namespace z
{
	namespace util
	{
		namespace rgx
		{
			/**
			* \brief Regex rule for matching a group of rules.
			*/
			class andlist : public compound
			{
			public:
				bool match(core::inputStream& stream) const noexcept;

#			ifdef DEBUG
				void print(core::outputStream& stream, int level=0) noexcept;
#			endif
			};
		}
	}
}
