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

			///Destructor
			~rgxmatcher();

			/**
			 * \brief Set a regex flag's state.
			 *
			 * \param flag The flag to set.
			 * \param state The state to set the flag to.
			 *
			 * \threadsafe_member_no
			 */
			void setFlag(rgxflag flag, bool state);

			/**
			 * \brief Get a regex flag's state.
			 *
			 * \param flag The flag to get.
			 *
			 * \return The state of the flag.
			 *
			 * \threadsafe_member_yes
			 */
			bool getFlag(rgxflag flag) const;

			/**
			 * \brief Copy the state of all current flags, and push them onto the stack.
			 * \threadsafe_member_no
			 */
			void pushFlags();

			/**
			 * \brief Pop the old flag states off the stack, replacing all current flags.
			 * \threadsafe_member_no
			 */
			void popFlags();

			/**
			 * \brief Get the number of bytes the given number of characters would consume.
			 *
			 * \param charCount The number of characters.
			 *
			 * \return 2 if this matcher's format is UTF16, 4 if it's UTF32, 1 otherwise.
			 *
			 * \threadsafe_member_yes
			 */
			size_t charsToOffset(size_t charCount) const;
		};
	}
}
