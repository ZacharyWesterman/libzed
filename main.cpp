// #define Z_USE_NARROW

// #include <z/system.h>
// #include <z/util.h>
// #include <z/core.h>
// #include <z/file.h>

#include <iostream>

// #include <z/core/array.h>
#include <z/core/string.h>
#include <z/core/stream.h>
using namespace z;

class console : public core::stream
{
public:
	void put(uint8_t ch)
	{
		std::cout << ch;
	}

	void put(uint8_t* str, size_t count, encoding format = ascii)
	{
		if (!str) return;
		uint8_t c[4];

		switch (format)
		{
		case utf16:
			for (size_t i=0; i<count; i++)
			{
				size_t len = core::toUTF8(c, ((uint16_t*)str)[i]);

				for (size_t j=0; j<len; j++)
					std::cout << c[j];
			}
			break;

		case utf32:
			for (size_t i=0; i<count; i++)
			{
				size_t len = core::toUTF8(c, ((uint32_t*)str)[i]);

				for (size_t j=0; j<len; j++)
					std::cout << c[j];
			}
			break;

		default:
			for (size_t i=0; i<count; i++)
			{
				std::cout << str[i];
			}
		}

	}

	uint8_t get()
	{
		return std::cin.get();
	}

	uint32_t getChar(encoding format = ascii)
	{
		return std::cin.get();
	}

	size_t get(size_t count, uint8_t* buf, encoding format = ascii)
	{
		return 0;
	}

	bool empty()
	{
		return false;
	}

	void seek(size_t index)
	{

	}

	size_t tell()
	{
		return 0;
	}

	size_t end()
	{
		return 0;
	}
};

int main()
{
	core::string<ascii> A;
	core::string<ascii> B = ";";
	console con;

	A.readln(con);
	// std::cout << ';' << A.length() << ';';

	(A += B).writeln(con);

	// A.writeln(B);

    return 0;
}
