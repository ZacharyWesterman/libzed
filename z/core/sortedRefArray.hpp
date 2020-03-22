#pragma once

#include <type_traits>

#include "sortedArray.hpp"

namespace z
{
	namespace core
	{
		/**
		 * \brief An extension of the core::sortedArray class,
		 * specialized for pointers.
		 *
		 * Unlike with the sortedArray, elements are
		 * assumed to be pointers and are sorted
		 * with this in mind.
		 *
		 * \note Rather than deal with the diamond problem or use
		 * a C++ version > C++11, this class inherits from sortedArray
		 * and reimplements refArray.
		 *
		 * \see array
		 * \see refArray
		 * \see sortedArray
		 */
		template <typename T>
		class sortedRefArray : public sortedArray<T>
		{
		private:
			//only allow this class to work with pointers
			static_assert(std::is_pointer<T>::value, "Template must be of pointer type.");

		protected:
			virtual bool eq(const T& arg1, const T& arg2) const override
			{
				return (arg1 == arg2) || (arg1 && arg2 && equals(*arg1, *arg2));
			}

			virtual bool gt(const T& arg1, const T& arg2) const override
			{
				if (arg1 == arg2) return false;
				else if (arg1 && arg2) return greater(*arg1, *arg2);
				else return arg1;
			}

			virtual bool lt(const T& arg1, const T& arg2) const override
			{
				if (arg1 == arg2) return false;
				else if (arg1 && arg2) return lesser(*arg1, *arg2);
				else return arg2;
			}

		public:
			sortedRefArray() {}

			template <typename... Args>
			sortedRefArray(const T& arg1, const Args&... args)
			{
				this->init(arg1, args...);
			}
		};
	}
}
