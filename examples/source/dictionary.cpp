#include <z/util/dictionary.hpp>
#include <z/file/inputStream.hpp>
#include <iostream>

int main()
{
	z::util::dictionary dict;
	z::file::inputStream file ("words_alpha.txt");
	dict.read(file, -1, true);
	std::cout << "Loaded" << std::endl;

	auto x = dict.range();

	for (auto i : zstring("fancy"))
	{
		auto res = dict.narrow(&x, i);
		std::cout << res << ' ' << x.isWord << ' ' << dict.word(x) << std::endl;
	}

	return 0;

	auto list = dict.decompose("fancypedestrian");
	for (auto& words : list)
	{
		// bool skip = false;
		// for (auto& word : words)
		// {
		// 	if (word.length() < 2)
		// 	{
		// 		skip = true;
		// 		break;
		// 	}
		// }
		// if (skip) continue;

		std::cout << words.length() << " -> ";
		for (auto& word : words)
		{
			std::cout << word << ", ";
		}
		std::cout << std::endl;
	}
}
