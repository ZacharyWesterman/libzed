#include "word.hpp"
#include <z/core/charFunctions.hpp>

namespace z
{
	namespace util
	{
		word::word() : part_of_speech(unknown) {}

		word::word(const core::string<>& name, part speechPart)
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

		const core::string<>& word::get() const
		{
			return _word;
		}

		void word::set(const core::string<>& name)
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

			for (int i=0; i<_word.length(); i++)
			{
				if (core::toUpper(_word[i]) != core::toUpper(other._word[i]))
					return false;
			}

			return true;
		}

		bool word::operator>(const word& other) const
		{
			int max_char;
			if (_word.length() < other._word.length())
				max_char = _word.length();
			else
				max_char = other._word.length();

			for (int i=0; i<max_char; i++)
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
			int max_char;
			if (_word.length() < other._word.length())
				max_char = _word.length();
			else
				max_char = other._word.length();

			for (int i=0; i<max_char; i++)
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

		size_t word::size() const
		{
			return _word.size() + sizeof(part_of_speech);
		}

		void word::serialIn(core::inputStream& stream)
		{
			_word.serialIn(stream);
			part_of_speech = (part)stream.get();
		}

		void word::serialOut(core::outputStream& stream) const
		{
			_word.serialOut(stream);
			stream.put((uint8_t)part_of_speech);
		}
	}
}
