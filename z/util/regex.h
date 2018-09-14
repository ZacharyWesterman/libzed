#pragma once

#include <z/core/string.h>
#include <z/core/stream.h>

#include "regex/rgxid.h"
#include "regex/rgxerr.h"
#include "regex/rgxscan.h"
#include "regex/rgxlex.h"
#include "regex/rgxmatch.h"


namespace z
{
	namespace util
	{

		/**
		 * \brief A class for parsing and matching regular expressions.
		 *
		 * Note that because the class requires seeking locations in a stream,
		 * it will not work with the system::console class.
		 *
		 * RE-ENTRANCE: Multiple accesses to the same object can cause data races.
		 */
		template <encoding E>
		class regex
		{
		private:
			rgxll* root;
			rgxerr parseError;

			core::string<E> matchedString;

		public:

			regex();
			regex(const core::string<E>&);

			void setPattern(const core::string<E>&);

			bool match(core::inputStream&);

			bool good() const;
			bool bad() const;
			rgxerr error() const;
			core::string<E> errstr() const;

			const core::string<E>& matched() const;
		};

		/**
		 * \brief Empty constructor.
		 */
		template <encoding E>
		regex<E>::regex() : root(0), parseError(RGX_NO_ERROR) {}

		/**
		 * \brief Constructor from string pattern
		 *
		 * \param pattern The regex pattern to match against.
		 */
		template <encoding E>
		regex<E>::regex(const core::string<E>& pattern)
		{
			root = 0;
			core::array<rgxss> symbols; //intermediate

			parseError = rgxscan(pattern, symbols);
			if (!parseError)
			{
				parseError = rgxlex(symbols, root);
			}
		}

		/**
		 * \brief Set the matching pattern
		 *
		 * \param pattern The pattern to match against.
		 */
		template <encoding E>
		void regex<E>::setPattern(const core::string<E>& pattern)
		{
			if (root) delete root;
			root = 0;
			core::array<rgxss> symbols; //intermediate

			parseError = rgxscan(pattern, symbols);
			if (!parseError)
			{
				parseError = rgxlex(symbols, root);
			}
		}

		/**
		 * \brief Attempt to match the pattern from a stream.
		 *
		 * Reads from a seekable input stream. Note that this does seek within a stream but it does not consume input.
		 *
		 * \param stream The input stream to read from.
		 *
		 * \return True if the stream is seekable and the pattern matched at the current stream index. False otherwise.
		 */
		template <encoding E>
		bool regex<E>::match(core::inputStream& stream)
		{
			if (!root)
			{
				parseError = RGX_ERROR;
				return false;
			}

			matchedString = "";

			if (parseError) return false;
			if (stream.bad())
			{
				parseError = RGX_BAD_STREAM;
				return false;
			}

			if (!stream.seekable())
			{
				parseError = RGX_NOT_SEEKABLE;
				return false;
			}

			rgxmatcher matcher (&stream, root, E);

			size_t startIndex = stream.tell();
			bool result = rgxmatch(&matcher);
			size_t endIndex = stream.tell();

			stream.seek(startIndex);

			if (matcher.fail)
			{
				parseError = RGX_BAD_STREAM;
			}
			else
			{
				while (stream.tell() < endIndex)
				{
					matchedString += stream.getChar(E);
				}
			}

			return result;
		}

		/**
		 * \brief Get whether the regex pattern is valid.
		 *
		 * \return True if the pattern is valid, false otherwise.
		 */
		template <encoding E>
		bool regex<E>::good() const
		{
			return (bool)root;
		}

		/**
		 * \brief Get whether the regex pattern is invalid.
		 *
		 * \return False if the pattern is valid, true otherwise.
		 */
		template <encoding E>
		bool regex<E>::bad() const
		{
			return !root;
		}

		/**
		 * \brief Get the regex pattern error, if any.
		 *
		 * \return The code for the pattern error. If no error, this will be RGX_NO_ERROR, which equals 0.
		 */
		template <encoding E>
		rgxerr regex<E>::error() const
		{
			return parseError;
		}

		/**
		 * \brief Get a string indicating the regex pattern error.
		 *
		 * \return A message describing the first encountered regex error.
		 */
		template <encoding E>
		core::string<E> regex<E>::errstr() const
		{
			const char* msgs[] =
			{
				"No errors",
				"Regex error",
				"Mismatched parentheses",
				"Mismatched brackets",
				"Mismatched braces",
				"Braces contain invalid characters",
				"Unable to parse pattern",
				"Invalid hexadecimal characters",
				"Improper character range",
				"Bad position for count struct",
				"Improper count struct format",
				"Misplaced greed suppressor",
				"Ineffective greed suppressor",
				"Invalid lookahead symbol",
				"Invalid lookbehind symbol",
				"Unknown negative flag",
				"Unknown positive flag",
				"Unable to analyze stream",
				"Unable to seek stream indices"
			};

			return msgs[parseError];
		}

		/**
		 * \brief Get the last string that the regex pattern matched.
		 *
		 * \return The last string of characters that this regex matched.
		 */
		template <encoding E>
		const core::string<E>& regex<E>::matched() const
		{
			return matchedString;
		}
	}
}
