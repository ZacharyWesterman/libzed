#pragma once

#include <cstdint>
#include <cstddef>

#include "rgxid.h"
#include "rgxss.h"

#include <z/core/array.h>

namespace z
{
	namespace util
	{
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

			uint8_t id() const;

			uint32_t beg() const;
			uint32_t end() const;

			size_t min() const;
			size_t max() const;

			bool greedy() const;

			void setID(uint8_t newID);

			void setRange(uint32_t beginSymbol, uint32_t endSymbol);
			void setExact(uint32_t symbol);
			void setNoChars();

			void setCountRange(size_t min, size_t max);
			void setCountExact(size_t count);
			void setCountNoMax(size_t count);

			void setGreedy(bool status = true);

			rgxll* parent();
			rgxll* sibling();

			void addChild(rgxll*);
			void setSibling(rgxll*);
		};
	}
}
