#pragma once

#include "../core/string.hpp"
#include "../core/sortedRefArray.hpp"
#include "../core/timeout.hpp"

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
			zstring lang;
			core::sortedRefArray<const zstring*> wordList;

			bool readingStream;

			zstring* readWordFromStream(core::inputStream& stream) noexcept;

		public:
			/**
			 * \brief Constructor
			 * \param langName A name to help identify the language (optional).
			 */
			dictionary(const zstring& langName = "") noexcept : lang(langName), readingStream(false) {};

			///Destructor
			~dictionary() noexcept;

			/**
			 * \brief Empty the word list.
			 */
			void clear() noexcept;

			/**
			 * \brief Read this dictionary's word list as text from a stream.
			 *
			 * Note that for long dictionaries, it may take
			 * several seconds for this operation to complete!
			 * Hence the timeout method.
			 *
			 * \param stream The stream to read from.
			 * \param time Optional param to force return after time out.
			 * \param assumePresorted If false, the word list is sorted while it is read.
			 * If true, no sorting is done.
			 *
			 * \note Operating on an un-sorted dictionary may have undefined behavior!
			 * Only set assumePresorted to true if you know for sure your data is sorted already!
			 *
			 * \return A positive integer if finished reading successfully, 0 if not finished reading,
			 * a negative integer if read failed.
			 *
			 * \threadsafe_member_no
			 */
			int read(core::inputStream& stream, const core::timeout& time = -1, bool assumePresorted = false) noexcept;

			/**
			 * \brief Check if the given string is a valid word in the dictionary (case is ignored).
			 *
			 * \param word The word to search for.
			 *
			 * \return True if it is a valid word, false otherwise.
			 *
			 * \threadsafe_member_yes
			 */
			bool isWord(const zstring& word) const noexcept;

			/**
			 * \brief Locate a word in the dictionary.
			 *
			 * \param word The word to find.
			 *
			 * \return The index of the word if it exists in the dictionary, -1 otherwise.
			 *
			 * \threadsafe_member_yes
			 */
			int find(const zstring& word) const noexcept;

			/**
			 * \brief Get the word count of this dictionary.
			 *
			 * \return The number of words in this dictionary.
			 *
			 * \threadsafe_member_yes
			 */
			int length() const noexcept;

			/**
			 * \brief Add a word to the dictionary.
			 *
			 * If the word already exists, does nothing. Otherwise, creates a new word.
			 *
			 * \param word The word to add or update.
			 *
			 * \threadsafe_member_no
			 */
			void addWord(const zstring& word) noexcept;

			/**
			 * \brief Get the language of this dictionary.
			 *
			 * \return A reference to the language name.
			 *
			 * \threadsafe_member_yes
			 */
			const zstring& language() const noexcept;

			/**
			 * \brief Set the language of this dictionary.
			 *
			 * \param newLang The string to set as the language name.
			 */
			void setLanguage(const zstring& newLang) noexcept;

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

#		if __has_include(<cereal/cereal.hpp>)
			/**
			 * \brief Serialization output.
			 * \param ar The output archive.
			 */
			template <typename archive>
			void save(archive& ar) const
			{
				ar((size_t)wordList.length());
				for (int i=0; i<(int)wordList.length(); i++)
				{
					ar(*(wordList[i]));
				}
			}

			/**
			 * \brief Serialization input.
			 * \param ar The input archive.
			 */
			template <class archive>
			void load(archive& ar)
			{
				clear();
				size_t sz;
				ar(sz);
				wordList.increase(sz);

				for (int i=0; i<sz; i++)
				{
					zstring* data = new zstring;
					ar(*data);
					wordList.append(data);
				}
			}
#		endif
		};
	}
}
