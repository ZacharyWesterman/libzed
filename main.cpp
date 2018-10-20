#include <z/system.h>
#include <z/core.h>
#include <z/file.h>

int main()
{
	z::file::outputStream stream ("out.txt");

	z::encoding format = stream.format();

	z::core::string<z::utf32> message = L"Hello, world!";

	message.writeln(stream, format);
	stream.flush();

	return 0;
}
