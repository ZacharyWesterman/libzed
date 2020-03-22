#pragma once

#include <z/core/string.hpp>
#include <z/core/stream.hpp>

#include "regex/rgxerr.hpp"
#include "regex/rules/compound.hpp"

#include <memory>

namespace z
{
	namespace util
	{

		/**
		 * \brief A class for parsing and matching regular expressions.
		 *
		 * Note that because the class requires seeking locations in a stream,
		 * it will not work with the system::console class.
		 */
		class regex
		{
		private:
			std::shared_ptr<rgx::compound> root;
			rgxerr parseError;

			zstring matchedString;

		public:

			/**
			 * \brief Empty constructor.
			 */
			regex();

			/**
			 * \brief Constructor from string pattern
			 *
			 * \param pattern The regex pattern to match against.
			 */
			regex(const zstring& pattern);

			/**
			 * \brief Set the matching pattern
			 *
			 * \param pattern The pattern to match against.
			 *
			 * \threadsafe_member_no
			 */
			void set(const zstring& pattern);

			/**
			 * \brief Attempt to match the pattern from a stream.
			 *
			 * Reads from a seekable input stream. Note that this does seek within a stream but it does not consume input.
			 *
			 * \param stream The input stream to read from.
			 *
			 * \return True if the stream is seekable and the pattern matched at the current stream index. False otherwise.
			 *
			 * \threadsafe_member_no
			 */
			bool match(core::inputStream& stream);

			/**
			 * \brief Get whether the regex pattern is valid.
			 *
			 * \return True if the pattern is valid, false otherwise.
			 *
			 * \threadsafe_member_yes
			 */
			bool good() const;

			/**
			 * \brief Get whether the regex pattern is invalid.
			 *
			 * \return False if the pattern is valid, true otherwise.
			 *
			 * \threadsafe_member_yes
			 */
			bool bad() const;

			/**
			 * \brief Get the regex pattern error, if any.
			 *
			 * \return The code for the pattern error. If no error, this will be RGX_NO_ERROR, which equals 0.
			 *
			 * \threadsafe_member_yes
			 */
			rgxerr error() const;

			/**
			 * \brief Get the last string that the regex pattern matched.
			 *
			 * \return The last string of characters that this regex matched.
			 *
			 * \threadsafe_member_yes
			 */
			const zstring& matched() const;

			/**
			 * \brief Get a string indicating the regex pattern error.
			 *
			 * \return A message describing the first encountered regex error.
			 *
			 * \threadsafe_member_yes
			 */
			zstring errorString() const;
		};
	}
}
