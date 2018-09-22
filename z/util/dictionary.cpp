#include "dictionary.h"

#include <z/file/exists.h>
#include <z/file/inputStream.h>

#include <z/system/console.h>

namespace z
{
	namespace util
	{
		bool dictionary::read(const core::string<utf8>& fileName)
		{
			system::console console;

			// core::string<utf32> test = file::exists(fileName);
			// test.writeln(console);

			// if (!file::exists(fileName)) return false;

			file::inputStream stream (fileName);
			const encoding format = stream.format();

			if (stream.bad()) return false;

			union
			{
				core::string<ascii>* asc;
				core::string<utf8>* ut8;
				core::string<utf16>* ut16;
				core::string<utf32>* ut32;
			} str;

			if (format == ascii)
				str.asc = new core::string<ascii>;
			else if (format == utf8)
				str.ut8 = new core::string<utf8>;
			else if (format == utf16)
				str.ut16 = new core::string<utf16>;
			else
				str.ut32 = new core::string<utf32>;

			while (!stream.empty())
			{
				core::string<utf32> result;

				if (format == ascii)
				{
					str.asc->read(stream);
					result = *str.asc;
				}
				else if (format == utf8)
				{
					str.ut8->read(stream);
					result = *str.ut8;
				}
				else if (format == utf16)
				{
					str.ut16->read(stream);
					result = *str.ut16;
				}
				else
				{
					str.ut32->read(stream);
					result = *str.ut32;
				}



				// core::string<utf8>(wordList.add(word(result))).writeln(console);
				wordList.add(word(result));
				// result.writeln(console);
			}

			if (format == ascii)
				delete str.asc;
			else if (format == utf8)
				delete str.ut8;
			else if (format == utf16)
				delete str.ut16;
			else
				delete str.ut32;

			return true;
		}

		bool dictionary::isWord(const core::string<utf32>& name) const
		{
			word check(name);

			return (wordList.find(check) >= 0);
		}

		word dictionary::getWord(const core::string<utf32>& name) const
		{
			word check(name);

			int index = wordList.find(check);
			if (index >= 0)
				return wordList[index];

			check.set(core::string<utf32>());
			return check;
		}

		size_t dictionary::wordCount() const
		{
			return wordList.length();
		}
	}
}
