#pragma once

class rgxss
{
private:
	uint8_t rgxID;
	uint32_t rgxSym;

public:
	rgxss(const uint8_t regexID, const uint32_t regexSymbol) : rgxID(regexID), rgxSym(regexSymbol) {}
	rgxss(const uint8_t regexID) : rgxID(regexID), rgxSym(0) {}

	const uint32_t symbol() const {return rgxSym;}
	const uint8_t id() const {return rgxID;}

	bool operator==(const rgxss& other) const {return (rgxID == other.rgxID);}
	// bool operator>(const rgxss& other) {return (rgxID > other.rgxID);}
	// bool operator<(const rgxss& other) {return (rgxID < other.rgxID);}
	const rgxss& operator=(const rgxss& other)
	{
		rgxID=other.rgxID;
		rgxSym=other.rgxSym;
		return *this;
	}
};
