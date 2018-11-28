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
			RGX_FLAG_CASEI, //case-sensitivity
			RGX_FLAG_NEWLINE,//whether . matches newlines
			RGX_FLAG_ENDLINE,//whether $ matches newlines

			RGX_FLAG_COUNT
		};
	}
}
