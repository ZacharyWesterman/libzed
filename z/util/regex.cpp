#include "regex.hpp"

#include "regex/rgxscan.hpp"
#include "regex/rgxlex.hpp"

namespace z
{
	namespace util
	{
		regex::regex() : root(NULL), parseError(RGX_NO_ERROR) {}

		regex::regex(const zstring& pattern) : root(NULL)
		{
			set(pattern);
		}

		void regex::set(const zstring& pattern)
		{
			if (root)
			{
				delete root;
				root = NULL;
			}

			core::array<rgxss> symbols;
			parseError = rgxscan(pattern, symbols);

			if (!parseError)
			{
				size_t position = 0;
				parseError = rgxlex(symbols, &root, position);
				if (parseError && root)
				{
					delete root;
					root = NULL;
				}
			}
		}

		bool regex::match(core::inputStream& stream)
		{
			if (!root) return false;

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

			matchedString.clear();
			auto begin = stream.tell();
			bool result = root->match(stream);

			if (result)
			{
				auto end = stream.tell();
				stream.seek(begin);
				while (stream.tell() < end)
				{
					matchedString.append(stream.getChar());
				}
			}

			return result;
		}

		bool regex::good() const
		{
			return (bool)root;
		}

		bool regex::bad() const
		{
			return !root;
		}

		rgxerr regex::error() const
		{
			return parseError;
		}

		zstring regex::errorString() const
		{
			const char* msgs[] =
			{
				"No errors",
				"Unknown regex error",
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
				"Unable to seek stream indices",
			};

			if (!(root || parseError)) return "No pattern specified";
			if (parseError >= RGX_ERR_COUNT) return msgs[1];
			return msgs[parseError];
		}

		const zstring& regex::matched() const
		{
			return matchedString;
		}
	}
}
