#include "dictionary.h"

#include <z/file/exists.h>
#include <z/file/inputStream.h>

#include <z/system/console.h>

#include <z/core/timer.h>

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

			core::string<utf32>* result;
			if (format == ascii)
			{
				str.asc->read(stream);
				result = new core::string<utf32>(*str.asc);
			}
			else if (format == utf8)
			{
				str.ut8->read(stream);
				result = new core::string<utf32>(*str.ut8);
			}
			else if (format == utf16)
			{
				str.ut16->read(stream);
				result = new core::string<utf32>(*str.ut16);
			}
			else
			{
				str.ut32->read(stream);
				result = new core::string<utf32>(*str.ut32);
			}

			// size_t read_count = 0;
			// core::timer time;

			while (!stream.empty())
			{
				wordList.add(result);
				// core::string<utf8> res = wordList.findInsert(result);

				// res.writeln(console);
				// read_count++;
				// if (!(wordList.length() % 1000))
				// {
				// 	core::string<utf8>(time.micros() / read_count).writeln(console);
				// }

				if (format == ascii)
				{
					str.asc->read(stream);
					result = new core::string<utf32>(*str.asc);
				}
				else if (format == utf8)
				{
					str.ut8->read(stream);
					result = new core::string<utf32>(*str.ut8);
				}
				else if (format == utf16)
				{
					str.ut16->read(stream);
					result = new core::string<utf32>(*str.ut16);
				}
				else
				{
					str.ut32->read(stream);
					result = new core::string<utf32>(*str.ut32);
				}


				// wordList.add(result);



				// if (!(wordList.length() % 1000))
				// core::string<utf8>(wordList.add(result)).writeln(console);
				// core::string<utf8>(wordList.length()).writeln(console);
				// result.writeln(console);
			}

			delete result;

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
			// for (size_t i=0; i<wordList.length(); i++)
			// {
			// 	if (wordList[i] == name) return true;
			// }
			core::string<utf32>* check = new core::string<utf32>(name);
			int result = wordList.find(check);

			delete check;
			return result >= 0;
		}

		word dictionary::getWord(const core::string<utf32>& name) const
		{
			// word check(name);
			// auto check = name;
			core::string<utf32>* check = new core::string<utf32>(name);


			int index = wordList.find(check);


			if (index >= 0)
				return word(*(wordList[index]));

			// check.set(core::string<utf32>());
			return word(*check);
		}

		size_t dictionary::wordCount() const
		{
			return wordList.length();
		}

		void dictionary::print(size_t start, size_t count) const
		{
			system::console con;

			if (!wordList.isValid(start)) return;

			size_t end = start + count;
			if (end > wordList.length()) end = wordList.length();

			for (size_t i=start; i<end; i++)
			{
				wordList[i]->writeln(con);
			}
		}
	}
}
