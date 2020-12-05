#pragma once

#include "../core/string.hpp"
#include "../core/refArray.hpp"
#include "../core/sortedRefArray.hpp"
#include "../core/timeout.hpp"

#include "dictionary/word.hpp"
#include "dictionary/dictRange.hpp"
#include "../core/sortedRefArray.hpp"

namespace z
{
	namespace util
	{
		/**
		 * \brief A class for performing searches on a dictionary of words.
		 */
		class dictionary : public core::sizable
		{
		private:
			core::string<> lang;
			core::sortedRefArray<word*> wordList;
			// core::refArray<word*> wordList;

			encoding streamFormat;
			bool readingStream;

			word* readWordFromStream(core::inputStream& stream) noexcept;

		public:
			///Constructor
			dictionary() noexcept;

			///Destructor
			~dictionary() noexcept;

			/**
			 * \brief Empty the word list.
			 */
			void clear() noexcept;

			/**
			 * \brief Read this dictionary's word list as text from a stream.
			 *
			 * Note that for long dictionaries (i.e. English), it may take
			 * several seconds for this operation to complete!
			 *
			 * \param stream The stream to read from.
			 * \param time Optional param to force return after time out
			 *
			 * \return A positive integer if finished reading successfully, 0 if not finished reading,
			 * a negative integer if read failed.
			 *
			 * \threadsafe_member_ref
			 */
			int read(core::inputStream& stream, const core::timeout& time = -1) noexcept;

			/**
			 * \brief Check if the given string is a valid word in the dictionary (case is ignored).
			 *
			 * \param name The word to search for.
			 *
			 * \return True if it is a valid word, false otherwise.
			 *
			 * \threadsafe_member_yes
			 */
			bool isWord(const core::string<>& name) const noexcept;

			/**
			 * \brief Get the word information for the given dictionary word.
			 *
			 * \param name The word to get info for.
			 *
			 * \return Approprifoundate word information if this is a valid word, blank word info otherwise.
			 *
			 * \threadsafe_member_yes
			 */
			word getWord(const core::string<>& name) const noexcept;

			/**
			 * \brief Get the word count of this dictionary.
			 *
			 * \return The number of words in this dictionary.
			 *
			 * \threadsafe_member_yes
			 */
			int wordCount() const noexcept;

			/**
			 * \brief Set a word in the dictionary.
			 *
			 * If the word already exists, updates its info. Otherwise, creates a new word.
			 *
			 * \param newWord The word to add or update.
			 *
			 * \threadsafe_member_no
			 */
			void setWord(const word& newWord) noexcept;

			/**
			 * \brief Get the language of this dictionary.
			 *
			 * \return A reference to he language name.
			 *
			 * \threadsafe_member_yes
			 */
			const core::string<>& language() const noexcept;

			/**
			 * \brief Set the language of this dictionary.
			 *
			 * \param newLang The string to set as the language name.
			 */
			void setLanguage(const core::string<>& newLang) noexcept;

			size_t size() const noexcept;

			/**
			 * \brief Create a new range of words encompassing the whole dictionary.
			 *
			 * This is meant to generate the initial object, then narrow() should be
			 * called to narrow down the possible results.
			 *
			 * \return An empty dictRange object.
			 */
			dictRange range() const noexcept;

			/**
			 * \brief Narrow the results of a range given the next character.
			 *
			 * \param wordRange A reference to the range we're narrowing.
			 * \param nextChar The next character all words in the range must contain.
			 *
			 * \return true if the range can be narrowed further, false otherwise.
			 */
			bool narrow(dictRange* wordRange, uint32_t nextChar) const noexcept;
		};
	}
}
