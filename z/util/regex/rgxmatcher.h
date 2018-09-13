#pragma once

#include <stack>
#include <z/core/stream.h>
#include "rgxll.h"
#include "rgxflag.h"

namespace z
{
	namespace util
	{
		/**
		 * \brief Data structure for regex pattern matching.
		 */
		struct rgxmatcher
		{
			core::inputStream* stream;
			rgxll* node;
			encoding format;
			bool fail;

			bool* flags;
			std::stack<bool*> flagStack;

			rgxmatcher(core::inputStream* input, rgxll* root, encoding charType);

			~rgxmatcher();

			void setFlag(rgxflag flag, bool state);

			bool getFlag(rgxflag flag) const;

			void pushFlags();

			void popFlags();

			size_t charsToOffset(size_t charCount) const;

		};
	}
}
