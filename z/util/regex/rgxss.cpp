#include "rgxss.hpp"

namespace z
{
	namespace util
	{
		rgxss::rgxss(const uint8_t regexID, const uint32_t regexSymbol) noexcept : rgxID(regexID), rgxSym(regexSymbol) {}
		rgxss::rgxss(const uint8_t regexID) noexcept : rgxID(regexID), rgxSym(0) {}

		uint32_t rgxss::symbol() const noexcept {return rgxSym;}
		uint8_t rgxss::id() const noexcept {return rgxID;}

		bool rgxss::operator==(const rgxss& other) const noexcept {return (rgxID == other.rgxID);}
		// bool operator>(const rgxss& other) {return (rgxID > other.rgxID);}
		// bool operator<(const rgxss& other) {return (rgxID < other.rgxID);}
		const rgxss& rgxss::operator=(const rgxss& other) noexcept
		{
			rgxID=other.rgxID;
			rgxSym=other.rgxSym;
			return *this;
		}
	}
}
