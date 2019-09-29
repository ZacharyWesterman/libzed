#include <z/core/string.h>
#include <z/file/inputStream.h>
#include <z/system/console.h>

int main()
{
	z::system::console console;
	z::file::inputStream in ("../data/in1.txt");
	z::core::string<z::utf8> text;

	if (in.bad())
	{
		zstring("File missing!").writeln(console);
		return 1;
	}

	//Note that in.empty() will be false until we try to read past the end of the stream.
	//So if we read individual characters from the stream, we should check whether the
	//stream is empty BEFORE processing the character.
	while(true)
	{
		auto ch = in.getChar();
		if (in.empty()) break;


		if (ch > 32)
			text = ch;
		else if (ch == 32)
			text = "˽";
		else if (ch == '\n')
			text = "˥";
		else if (ch == '\r')
			text = "˿";
		else if (ch == '\t')
			text = "˾";
		else
			text =(int)ch;

		if (ch == '\n')
			text.writeln(console);
		else
			text.write(console);
	}

	//However, if we're reading lines from a stream, we should not just exit when we pass
	//the end of the stream. Some streams may not end in a newline!
	//A better way would be to check if the result string's length is 0.
	//A length of 0 would mean we were not able to pull anything off the stream.
	in.seek(0);

	console.put('\n');
	while(true)
	{
		text.readln(in);
		if (!text.length()) break;

		text.writeln(console);
	}

	return 0;
}
