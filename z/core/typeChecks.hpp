#pragma once

namespace z
{
	namespace core
	{
		namespace types
		{
			///Internal class to do template check if method exists.
			class no { bool b[2]; };
			template<typename T, typename Arg> no operator== (const T&, const Arg&);
			template<typename T, typename Arg> no operator< (const T&, const Arg&);
			template<typename T, typename Arg> no operator> (const T&, const Arg&);

			bool check (...);
			no& check (const no&);

			///Template check if operator== exists
			template <typename T, typename Arg = T>
			struct equalExists
			{
				enum { value = (sizeof(check(*(T*)(0) == *(Arg*)(0))) != sizeof(no)) };
			};

			///Template check if operator< exists
			template <typename T, typename Arg = T>
			struct lesserExists
			{
				enum { value = (sizeof(check(*(T*)(0) < *(Arg*)(0))) != sizeof(no)) };
			};

			///Template check if operator> exists
			template <typename T, typename Arg = T>
			struct greaterExists
			{
				enum { value = (sizeof(check(*(T*)(0) > *(Arg*)(0))) != sizeof(no)) };
			};

			///Template check if class does not exist
			template <class T, class Enable = void>
			struct isDefined
			{
				static constexpr bool value = false;
			};

			///Template check if class exists
			template <class T>
			struct isDefined<T, std::enable_if_t<(sizeof(T) > 0)>>
			{
				static constexpr bool value = true;
			};
		}
	}
}
