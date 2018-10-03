#pragma once

#ifndef Z_DICT_FORMAT
#define Z_DICT_FORMAT z::utf8
#endif

namespace z
{
	namespace util
	{
		/**
		 * \brief Flags indicating what parts of speech a word belongs to.
		 */
		enum part
		{
			unknown      = 0x0000,
			noun         = 0x0001,
			verb         = 0x0002,
			adjective    = 0x0004,
			adverb       = 0x0008,
			pronoun      = 0x0010,
			preposition  = 0x0020,
			conjunction  = 0x0040,
			determiner   = 0x0080,
			interjection = 0x0100,
		};
	}
}
