#pragma once

// #include <type_traits>
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
			rgxss(const uint8_t regexID, const uint32_t regexSymbol);
			rgxss(const uint8_t regexID);

			const uint32_t symbol() const;
			const uint8_t id() const;

			bool operator==(const rgxss& other) const;
			// bool operator>(const rgxss& other) {return (rgxID > other.rgxID);}
			// bool operator<(const rgxss& other) {return (rgxID < other.rgxID);}
			const rgxss& operator=(const rgxss& other);
		};
	}
}
