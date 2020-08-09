#pragma once

namespace z
{
	namespace util
	{
		class dictionary;

		class dictRange
		{
		protected:
			int left;
			int right;
			int charPos;

		public:
			bool exhausted;
			friend dictionary;
		};
	}
}
