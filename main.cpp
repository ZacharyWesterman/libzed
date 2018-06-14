#define Z_USE_NARROW

#include <z/system.h>
#include <z/util.h>
#include <z/core.h>
#include <z/file.h>

using namespace z;

template <typename T>
void print(core::inputStream<T>& stream, core::outputStream<char>& con)
{
	// system::console con;

	con.writeln("{");

	Int col = 1;

	stream.seek(0);
	while(!stream.empty())
	{
		core::string<char> pad = stream.get();
		for (Int i=pad.length(); i<5; i++)
			pad += " ";

		con.write(cs("  ")+pad);
		if (!(col % 5))
		{
			con.writeln();
			col = 0;
		}
		col++;
	}

	if (col % 5) con.writeln();

	con.writeln("}");
}

int main()
{
	// system::console con;
	file::outputStream<char> con ("out.txt");

	core::binaryStream<int> stream;

	int intl[] = {0,1,1,2,3,5,8,13,21,34,55};

	for (int i=0; i<11; i++)
		stream.put(intl[i]);

	// con.writeln(stream.get());

	print(stream, con);

    return 0;
}
