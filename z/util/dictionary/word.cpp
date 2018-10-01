#include "word.h"
#include <z/core/charFunctions.h>

namespace z
{
	namespace util
	{
		word::word() : part_of_speech(unknown) {}

		word::word(const core::string<utf32>& name, part speechPart)
		{
			_word = name;
			part_of_speech = speechPart;
		}

		word::word(const word& other)
		{
			_word = other._word;
			part_of_speech = other.part_of_speech;
		}

		const word& word::operator=(const word& other)
		{
			_word = other._word;
			part_of_speech = other.part_of_speech;

			return *this;
		}

		const core::string<utf32>& word::get() const
		{
			return _word;
		}

		void word::set(const core::string<utf32>& name)
		{
			_word = name;
		}

		part word::getPart() const
		{
			return part_of_speech;
		}

		void word::setPart(part speechPart)
		{
			part_of_speech = speechPart;
		}

		bool word::operator==(const word& other) const
		{
			if (_word.length() != other._word.length())
				return false;

			for (size_t i=0; i<_word.length(); i++)
			{
				if (core::toUpper(_word[i]) != core::toUpper(other._word[i]))
					return false;
			}

			return true;
		}

		bool word::operator>(const word& other) const
		{
			size_t max_char;
			if (_word.length() < other._word.length())
				max_char = _word.length();
			else
				max_char = other._word.length();

			for (size_t i=0; i<max_char; i++)
			{
				auto char1 = core::toUpper(_word[i]);
				auto char2 = core::toUpper(other._word[i]);

				if (char1 > char2)
					return true;
				else if (char1 < char2)
					return false;
			}

			return (_word.length() > other._word.length());
		}

		bool word::operator<(const word& other) const
		{
			size_t max_char;
			if (_word.length() < other._word.length())
				max_char = _word.length();
			else
				max_char = other._word.length();

			for (size_t i=0; i<max_char; i++)
			{
				auto char1 = core::toUpper(_word[i]);
				auto char2 = core::toUpper(other._word[i]);

				if (char1 > char2)
					return false;
				else if (char1 < char2)
					return true;
			}

			return (_word.length() < other._word.length());
		}
	}
}
