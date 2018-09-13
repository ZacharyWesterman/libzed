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
			///The stream to read from.
			core::inputStream* stream;

			///The current root node to match against.
			rgxll* node;

			///The character format to read from the stream.
			encoding format;

			///Flag indicating whether an unexpected error was encountered.
			bool fail;

			///The list of flags that apply to the current node.
			bool* flags;

			///Stack holding the flags of previous nodes.
			std::stack<bool*> flagStack;

			/**
			 * \brief Constructor.
			 *
			 * \param input The stream to read from.
			 * \param root The starting root node.
			 * \param charType The encoding of characters in the stream; e.g. UTF-8
			 */
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
