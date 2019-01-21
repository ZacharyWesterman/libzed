#pragma once

#include <cstdint>

namespace z
{
	namespace util
	{
		/**
		 * \brief Regex pattern scan symbol
		 */
		class rgxss
		{
		private:
			uint8_t rgxID;
			uint32_t rgxSym;

		public:

			/**
			 * \brief ID and character constructor.
			 *
			 * \param regexID The ID of this symbol.
			 * \param regexSymbol An additional character associated with this symbol.
			 */
			rgxss(const uint8_t regexID, const uint32_t regexSymbol);

			/**
			 * \brief Constructor from ID.
			 *
			 * \param regexID The ID of this symbol.
			 */
			rgxss(const uint8_t regexID);

			/**
			 * \brief Get this symbol's character.
			 * \return The character associated with this symbol.
			 * \threadsafe_member_yes
			 */
			uint32_t symbol() const;

			/**
			 * \brief Get this symbol's ID.
			 * \return The ID of this symbol.
			 * \threadsafe_member_yes
			 */
			uint8_t id() const;

			/**
			 * \brief Equality operator.
			 *
			 * Provided to allow searching in core::array.
			 *
			 * \param other Symbol to compare against.
			 * \return True if the symbols are equivalent, false otherwise.
			 * \threadsafe_member_yes
			 */
			bool operator==(const rgxss& other) const;

			/**
			 * \brief Assignment operator.
			 *
			 * \param other Symbol to assign as.
			 * \return A reference to this symbol after assignment.
			 * \threadsafe_member_no
			 */
			const rgxss& operator=(const rgxss& other);
		};
	}
}
