#pragma once

namespace z
{
	namespace core
	{
		namespace types
		{
			class no { bool b[2]; };
			template<typename T, typename Arg> no operator== (const T&, const Arg&);
			template<typename T, typename Arg> no operator< (const T&, const Arg&);
			template<typename T, typename Arg> no operator> (const T&, const Arg&);

			bool check (...);
			no& check (const no&);

			template <typename T, typename Arg = T>
			struct equalExists
			{
				enum { value = (sizeof(check(*(T*)(0) == *(Arg*)(0))) != sizeof(no)) };
			};

			template <typename T, typename Arg = T>
			struct lesserExists
			{
				enum { value = (sizeof(check(*(T*)(0) < *(Arg*)(0))) != sizeof(no)) };
			};

			template <typename T, typename Arg = T>
			struct greaterExists
			{
				enum { value = (sizeof(check(*(T*)(0) > *(Arg*)(0))) != sizeof(no)) };
			};
		}
	}
}
