#include "rgxmatcher.h"

namespace z
{
	namespace util
	{
		rgxmatcher::rgxmatcher(core::inputStream* input, rgxll* root, encoding charType)
		{
			fail = !(input && root);
			stream = input;
			node = root;
			format = charType;

			flags = new bool[RGX_FLAG_COUNT];
			for (size_t i=0; i<RGX_FLAG_COUNT; i++)
			{
				flags[i] = false;
			}
		}

		rgxmatcher::~rgxmatcher()
		{
			delete[] flags;

			while (!flagStack.empty())
			{
				delete[] flagStack.top();
				flagStack.pop();
			}
		}

		void rgxmatcher::setFlag(rgxflag flag, bool state)
		{
			if ((flag > RGX_FLAG_COUNT) || (flag < 0)) return;

			flags[flag] = state;
		}

		bool rgxmatcher::getFlag(rgxflag flag) const
		{
			if ((flag > RGX_FLAG_COUNT) || (flag < 0)) return false;

			return flags[flag];
		}

		void rgxmatcher::pushFlags()
		{
			bool* newFlags = new bool[RGX_FLAG_COUNT];
			for (size_t i=0; i<RGX_FLAG_COUNT; i++)
			{
				newFlags[i] = flags[i];
			}

			flagStack.push(flags);
			flags = newFlags;
		}

		void rgxmatcher::popFlags()
		{
			delete[] flags;
			flags = flagStack.top();
			flagStack.pop();
		}

		size_t rgxmatcher::charsToOffset(size_t charCount) const
		{
			switch (format)
			{
				case utf32:
					return charCount << 2;
				case utf16:
					return charCount << 1;
				default:
					return charCount;
			}
		}
	}
}
