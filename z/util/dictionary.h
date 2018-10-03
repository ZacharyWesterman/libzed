#pragma once

#include <z/core/string.h>
#include <z/core/sortedRefArray.h>

#include "dictionary/word.h"

namespace z
{
	namespace util
	{
		/**
		 * \brief A class for performing searches on a dictionary of words.
		 */
		class dictionary : public core::sizable, public core::serializable
		{
		private:
			core::string<Z_DICT_FORMAT> lang;
			core::sortedRefArray<word*> wordList;

		public:
			///Destructor
			~dictionary();

			/**
			 * \brief Empty the word list.
			 */
			void clear();

			/**
			 * \brief Read this dictionary's word list from a text file.
			 *
			 * Note that for long dictionaries (i.e. English), it may take
			 * several seconds for this operation to complete!
			 *
			 * \param fileName The name of the file to read from.
			 *
			 * \return True if read succeeded, false otherwise.
			 */
			bool read(const core::string<utf8>& fileName);

			/**
			 * \brief Check if the given string is a valid word in the dictionary (case is ignored).
			 *
			 * \param name The word to search for.
			 *
			 * \return True if it is a valid word, false otherwise.
			 */
			bool isWord(const core::string<Z_DICT_FORMAT>& name) const;

			/**
			 * \brief Get the word information for the given dictionary word.
			 *
			 * \param name The word to get info for.
			 *
			 * \return Appropriate word information if this is a valid word, blank word info otherwise.
			 */
			word getWord(const core::string<Z_DICT_FORMAT>& name) const;

			/**
			 * \brief Get the word count of this dictionary.
			 *
			 * \return The number of words in this dictionary.
			 */
			size_t wordCount() const;

			size_t size() const;

			void serialIn(core::inputStream& stream);

			void serialOut(core::outputStream& stream) const;
		};
	}
}
