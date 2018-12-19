#include <z/system.h>
#include <z/core.h>
#include <z/file.h>

int main()
{
	z::system::console console;
	z::file::outputStream out("out.txt", z::utf32, true);
	// z::file::inputStream in("out.txt");

	// zpath(in.good() ? "good" : "bad").writeln(console);

	z::core::string<z::utf8> str = "test";
	// str.read(in);
	// switch (in.format())
	// {
	// 	case z::ascii:
	// 		str = "ascii";
	// 	break;
	//
	// 	case z::utf8:
	// 		str = "utf8";
	// 	break;
	//
	// 	case z::utf16:
	// 		str = "utf16";
	// 	break;
	//
	// 	case z::utf32:
	// 		str = "utf32";
	// 	break;
	//
	// 	default:
	// 		str = "unknown";
	// }

	str.writeln(out);
	// z::core::string<z::utf16>("hello, this is a test.").writeln(out);

	return 0;
}
