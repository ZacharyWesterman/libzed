#pragma once

#include <z/core/string.h>

#include "part.h"

namespace z
{
	namespace util
	{
		class word
		{
		private:
			core::string<utf32> _word;
			part part_of_speech;

		public:
			word();
			word(const core::string<utf32>& name, part speechPart = unknown);
			word(const word& other);
			word(word&& other);

			const word& operator=(const word& other);

			const core::string<utf32>& get() const;

			void set(const core::string<utf32>& name);

			part getPart() const;

			void setPart(part speechPart);

			bool operator==(const word& other) const;
			bool operator>(const word& other) const;
			bool operator<(const word& other) const;
		};
	}
}
