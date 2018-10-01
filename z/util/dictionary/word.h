#pragma once

#include <z/core/string.h>

#include "part.h"

namespace z
{
	namespace util
	{
		/**
		 * \brief Container holding information about a word.
		 */
		class word
		{
		private:
			core::string<utf32> _word;
			part part_of_speech;

		public:
			///Default constructor.
			word();

			/**
			 * \brief Full constructor.
			 *
			 * \param name The word.
			 * \param speechPart The part of speech that this word is.
			 */
			word(const core::string<utf32>& name, part speechPart = unknown);

			/**
			 * \brief Copy constructor.
			 *
			 * \param other The word to duplicate.
			 */
			word(const word& other);

			/**
			 * \brief Assignment operator.
			 *
			 * \param other The word to copy from.
			 *
			 * \return A reference to this word after assignment.
			 */
			const word& operator=(const word& other);

			/**
			 * \brief Get the text pertaining to this word.
			 *
			 * \return A reference to this word's text.
			 */
			const core::string<utf32>& get() const;

			/**
			 * \brief Set this word's text.
			 *
			 * \param name The word.
			 */
			void set(const core::string<utf32>& name);

			/**
			 * \brief Get the part of speech this word is.
			 *
			 * \return The assigned speech part for this word.
			 */
			part getPart() const;

			/**
			 * \brief Set this word's part of speech.
			 *
			 * \param speechPart The part of speech this word is.
			 */
			void setPart(part speechPart);

			/**
			 * \brief Equality operator (case is ignored).
			 *
			 * \param other The word to compare to.
			 *
			 * \return True if referencing the same word, false otherwise.
			 */
			bool operator==(const word& other) const;

			/**
			 * \brief Greater-than operator (case is ignored).
			 *
			 * \param other The word to compare to.
			 *
			 * \return True if this word comes after the other alphabetically, false otherwise.
			 */
			bool operator>(const word& other) const;

			/**
			 * \brief Less-than operator (case is ignored).
			 *
			 * \param other The word to compare to.
			 *
			 * \return True if this word comes before the other alphabetically, false otherwise.
			 */
			bool operator<(const word& other) const;
		};
	}
}
