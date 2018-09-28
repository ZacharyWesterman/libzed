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

		word::word(word&& other) : _word(other._word)
		{
			part_of_speech = other.part_of_speech;
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
			// return _word.upper() == other._word.upper();
			return _word.length() == other._word.length();
		}

		bool word::operator>(const word& other) const
		{
			// return _word.upper() > other._word.upper();
			return _word.length() > other._word.length();
		}

		bool word::operator<(const word& other) const
		{
			// return _word.upper() < other._word.upper();
			return _word.length() < other._word.length();
		}
	}
}
