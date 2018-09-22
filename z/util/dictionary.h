#pragma once

#include <z/core/sizable.h>
#include <z/core/serializable.h>
#include <z/core/string.h>
#include <z/core/sortedArray.h>

#include "dictionary/word.h"

namespace z
{
	namespace util
	{
		class dictionary
		{
		private:
			core::string<utf32> lang;
			core::sortedArray<word> wordList;

		public:
			bool read(const core::string<utf8>& fileName);

			bool isWord(const core::string<utf32>& name) const;

			word getWord(const core::string<utf32>& name) const;

			size_t wordCount() const;
		};
	}
}