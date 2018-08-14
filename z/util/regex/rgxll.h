#pragma once

class rgxll
{
private:
	rgxll* rgxParent;
	rgxll* rgxSibling;

	uint8_t rgxID;

	uint32_t symbolBeg;
	uint32_t symbolEnd;
	size_t minCount;
	size_t maxCount;

	bool isGreedy;

public:
	core::array<rgxll*> children;

	rgxll(uint8_t ID);
	rgxll(const rgxss& init);
	~rgxll();

	uint8_t id() const {return rgxID;}

	uint32_t beg() const {return symbolBeg;}
	uint32_t end() const {return symbolEnd;}

	size_t min() const {return minCount;}
	size_t max() const {return maxCount;}

	bool greedy() const {return isGreedy;}

	void setID(uint8_t newID) {rgxID = newID;}

	void setRange(uint32_t beginSymbol, uint32_t endSymbol);
	void setExact(uint32_t symbol);
	void setNoChars();

	void setCountRange(size_t min, size_t max);
	void setCountExact(size_t count);
	void setCountNoMax(size_t count);

	void setGreedy(bool status = true) {isGreedy = status;}

	rgxll* parent() {return rgxParent;}
	rgxll* sibling() {return rgxSibling;}

	void addChild(rgxll*);
	void setSibling(rgxll*);
};

rgxll::rgxll(uint8_t ID)
{
	rgxID = ID;
	symbolBeg = 0;
	symbolEnd = 0;
	minCount = 1;
	maxCount = 1;
	isGreedy = true;
	rgxParent = 0;
	rgxSibling = 0;
}

rgxll::rgxll(const rgxss& init)
{
	rgxID = init.id();
	symbolBeg = init.symbol();
	symbolEnd = init.symbol();
	minCount = 1;
	maxCount = 1;
	isGreedy = true;
	rgxParent = 0;
	rgxSibling = 0;
}

rgxll::~rgxll()
{
	for (size_t i=0; i<children.length(); i++)
		delete children[i];
}

void rgxll::setRange(uint32_t beginSymbol, uint32_t endSymbol)
{
	if (beginSymbol > endSymbol)
	{
		symbolBeg = endSymbol;
		symbolEnd = beginSymbol;
	}
	else
	{
		symbolBeg = beginSymbol;
		symbolEnd = endSymbol;
	}
}

void rgxll::setExact(uint32_t symbol)
{
	symbolBeg = symbol;
	symbolEnd = symbol;
}

void rgxll::setNoChars()
{
	symbolBeg = 0;
	symbolEnd = 0;
}

void rgxll::setCountRange(size_t min, size_t max)
{
	if (min > max)
	{
		minCount = max;
		maxCount = min;
	}
	else
	{
		minCount = min;
		maxCount = max;
	}
}

void rgxll::setCountExact(size_t count)
{
	minCount = count;
	maxCount = count;
}

void rgxll::setCountNoMax(size_t count)
{
	minCount = count;
	maxCount = -1;
}

void rgxll::addChild(rgxll* node)
{
	if (node)
	{
		node->rgxParent = this;
		children.add(node);
	}
}

void rgxll::setSibling(rgxll* node)
{
	rgxSibling = node;
}
