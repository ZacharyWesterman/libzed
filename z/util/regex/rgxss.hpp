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
			rgxss(const uint8_t regexID, const uint32_t regexSymbol) noexcept;

			/**
			* \brief Constructor from ID.
			*
			* \param regexID The ID of this symbol.
			*/
			rgxss(const uint8_t regexID = 0) noexcept;

			/**
			* \brief Get this symbol's character.
			* \return The character associated with this symbol.
			* \threadsafe_member_yes
			*/
			uint32_t symbol() const noexcept;

			/**
			* \brief Get this symbol's ID.
			* \return The ID of this symbol.
			* \threadsafe_member_yes
			*/
			uint8_t id() const noexcept;

			/**
			* \brief Equality operator.
			*
			* Provided to allow searching in core::array.
			*
			* \param other Symbol to compare against.
			* \return True if the symbols are equivalent, false otherwise.
			* \threadsafe_member_yes
			*/
			bool operator==(const rgxss& other) const noexcept;
		};
	}
}
