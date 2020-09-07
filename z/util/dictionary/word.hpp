#pragma once

#include <z/core/string.hpp>

#include <z/core/sizable.hpp>
#include <z/core/serializable.hpp>

#include "part.hpp"

namespace z
{
	namespace util
	{
		/**
		 * \brief Container holding information about a word.
		 */
		class word : public core::sizable, public core::serializable
		{
		private:
			core::string<> _word;
			part part_of_speech;

		public:
			///Default constructor.
			word() noexcept;

			/**
			 * \brief Full constructor.
			 *
			 * \param name The word.
			 * \param speechPart The part of speech that this word is.
			 */
			word(const core::string<>& name, part speechPart = unknown) noexcept;

			/**
			 * \brief Copy constructor.
			 *
			 * \param other The word to duplicate.
			 */
			word(const word& other) noexcept;

			/**
			 * \brief Assignment operator.
			 *
			 * \param other The word to copy from.
			 *
			 * \return A reference to this word after assignment.
			 *
			 * \threadsafe_member_no
			 */
			const word& operator=(const word& other) noexcept;

			/**
			 * \brief Get the text pertaining to this word.
			 *
			 * \return A reference to this word's text.
			 *
			 * \threadsafe_member_yes
			 */
			const core::string<>& get() const noexcept;

			/**
			 * \brief Set this word's text.
			 *
			 * \param name The word.
			 *
			 * \threadsafe_member_yes
			 */
			void set(const core::string<>& name) noexcept;

			/**
			 * \brief Get the part of speech this word is.
			 *
			 * \return The assigned speech part for this word.
			 *
			 * \threadsafe_member_yes
			 */
			part getPart() const noexcept;

			/**
			 * \brief Set this word's part of speech.
			 *
			 * \param speechPart The part of speech this word is.
			 *
			 * \threadsafe_member_no
			 */
			void setPart(part speechPart) noexcept;

			/**
			 * \brief Equality operator (case is ignored).
			 *
			 * \param other The word to compare to.
			 *
			 * \return True if referencing the same word, false otherwise.
			 *
			 * \threadsafe_member_yes
			 */
			bool operator==(const word& other) const noexcept;

			/**
			 * \brief Greater-than operator (case is ignored).
			 *
			 * \param other The word to compare to.
			 *
			 * \return True if this word comes after the other alphabetically, false otherwise.
			 *
			 * \threadsafe_member_yes
			 */
			bool operator>(const word& other) const noexcept;

			/**
			 * \brief Less-than operator (case is ignored).
			 *
			 * \param other The word to compare to.
			 *
			 * \return True if this word comes before the other alphabetically, false otherwise.
			 *
			 * \threadsafe_member_yes
			 */
			bool operator<(const word& other) const noexcept;

			size_t size() const noexcept;

			void serialIn(core::inputStream& stream);

			void serialOut(core::outputStream& stream) const;
		};
	}
}
