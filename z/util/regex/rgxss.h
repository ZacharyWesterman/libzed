#pragma once

class rgxss
{
private:
	uint8_t rgxID;
	uint32_t rgxSymbol;

public:
	rgxss(const uint8_t regexID, const uint32_t regexSymbol) : rgxID(regexID), rgxSym(regexSymbol) {}
	rgxss(const uint8_t regexID) : rgxID(regexID), rgxSymbol(0) {}

	const uint32_t symbol() const {return rgxSym;}
	const uint8_t id() const {return rgxID;}

	bool operator==(const rgxss& other) {return (rgxID == other.rgxID);}
	// bool operator>(const rgxss& other) {return (rgxID > other.rgxID);}
	// bool operator<(const rgxss& other) {return (rgxID < other.rgxID);}
};
