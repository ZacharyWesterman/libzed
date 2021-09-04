#pragma once

#include "stdin.hpp"
#include "stdout.hpp"

namespace z
{
	namespace system
	{
		/**
		* \brief Class for interacting with the console.
		*
		* This class provides an implementation of core::stream.
		* All read-related methods read from stdin, and all
		* write-related methods write to stdout. Note that
		* because of how stdin and stdout behave, you cannot
		* unget characters or seek a position in the stream;
		* those are just dummy methods.
		* As a result, this class is not compatible with the
		* util::regex class.
		*/
		class console : public stdin, public stdout {};
	}
}
