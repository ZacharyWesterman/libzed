#define Z_USE_NARROW

#include <z/system.h>
#include <z/util.h>
#include <z/core.h>
#include <z/file.h>

using namespace z;

/*template <typename T>
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
}*/

int main()
{
	system::console con;
	file::inputStream<char> stream ("out.byte");

	core::string<char> data;
	// core::string<char> data;

	core::serialIn(&data, &stream);
	// core::serialIn(&data, &stream);

	con.writeln(data);
	// con.writeln(data);
	// while (!output.empty())
		// con.writeln(output.read());

	// char tmp[10];

	// con.writeln(wctomb(tmp, L'it\'sƔtest'));
	// core::string<byte> data2 = data;

	// std::basic_ofstream<wchar_t> out("out.byte");
	// con.writeln(data);
	// out << ((byte)'A');
	// output.put('A');
	// output.write(data);
	// con.writeln(output.empty());
	// core::serialOut(data, &output);
	// core::binaryStream<int> stream;

	// std::wofstream out ("out.byte");

	// out << L"Ɣtest";

	// int intl[] = {0,1,1,2,3,5,8,13,21,34,55};

	// for (int i=0; i<11; i++)
		// stream.put(intl[i]);

	// print(stream, con);

    return 0;
}
