#include "rgxss.hpp"

namespace z
{
	namespace util
	{
		rgxss::rgxss(const uint8_t regexID, const uint32_t regexSymbol) : rgxID(regexID), rgxSym(regexSymbol) {}
		rgxss::rgxss(const uint8_t regexID) : rgxID(regexID), rgxSym(0) {}

		uint32_t rgxss::symbol() const {return rgxSym;}
		uint8_t rgxss::id() const {return rgxID;}

		bool rgxss::operator==(const rgxss& other) const {return (rgxID == other.rgxID);}
		// bool operator>(const rgxss& other) {return (rgxID > other.rgxID);}
		// bool operator<(const rgxss& other) {return (rgxID < other.rgxID);}
		const rgxss& rgxss::operator=(const rgxss& other)
		{
			rgxID=other.rgxID;
			rgxSym=other.rgxSym;
			return *this;
		}
	}
}
