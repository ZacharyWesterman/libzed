#include "rgxll.h"

namespace z
{
	namespace util
	{
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

		uint8_t rgxll::id() const {return rgxID;}

		uint32_t rgxll::beg() const {return symbolBeg;}
		uint32_t rgxll::end() const {return symbolEnd;}

		size_t rgxll::min() const {return minCount;}
		size_t rgxll::max() const {return maxCount;}

		bool rgxll::greedy() const {return isGreedy;}

		void rgxll::setID(uint8_t newID) {rgxID = newID;}

		void rgxll::setGreedy(bool status) {isGreedy = status;}

		rgxll* rgxll::parent() {return rgxParent;}
		rgxll* rgxll::sibling() {return rgxSibling;}
	}
}
