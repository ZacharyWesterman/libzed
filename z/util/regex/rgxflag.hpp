#pragma once

namespace z
{
	namespace util
	{
		/**
		* \brief Supported regex pattern flags.
		*/
		enum rgxflag
		{
			INSENSITIVE=1,
			NEWLINE=2,
			ENDLINE=4,
			NEGATE=8,
		};
	}
}
