#pragma once

#include <type_traits>

#include "array.h"

namespace z
{
	namespace core
	{
		/**
		 * \brief An extension of the core::array class which
		 * assumes all elements to be pointers.
		 *
		 * \see array
		 * \see sortedArray
		 */
		template <typename T>
		class refArray : public array<T>
		{
			//only allow this class to work with pointers
			static_assert(std::is_pointer<T>::value, "Template must be of pointer type.");

		public:
			///Default constructor.
			refArray() {}

			refArray(const array<T>&);
			refArray(const T);

			template <typename... Args>
			refArray(const T arg1, const Args... args);

			intmax_t find(const T& object) const;
		};


		///Copy constructor
		template <typename T>
		refArray<T>::refArray(const array<T>& other)
		{
			this->array_data = other.array_data;
		}

		/**
		 * \brief Constructor with one argument.
		 *
		 * Constructs the array with one
		 * element already contained.<BR>
		 *
		 * \b Syntax: array<T> X (arg1); array<T> X = arg1;
		 *
		 * \param arg1 initializing data.
		 */
		template <typename T>
		refArray<T>::refArray(const T arg1)
		{
			this->array_data.push_back(arg1);
		}


		/**
		 * \brief List-initialized constructor.
		 *
		 * Constructs the array with an arbitrary
		 * number of elements already contained.<BR>
		 *
		 * \b Syntax: array<T> X {arg1, arg2, ...};
		 * array<T> X = {arg1, arg2, ...};
		 *
		 * \param arg1 initializing data.
		 * \param args cont. initializing data.
		 */
		template <typename T>
		template <typename... Args>
		refArray<T>::refArray(const T arg1, const Args... args)
		{
			init(arg1, args...);
		}


		/**
		 * \brief Check if a given object with identical
		 * data is in the array.
		 *
		 * Locates the desired index using a linear search,
		 * as the array is expected to be unsorted.
		 *
		 * \param object a pointer to the object to search for.
		 *
		 * \return The first index that the object data was found at.
		 * \b -1 if it was not found.
		 */
		template <typename T>
		intmax_t refArray<T>::find(const T& object) const
		{
			for (intmax_t i=0; i<((int)this->array_data.size()); i++)
			{
				T obj1 = this->array_data.at(i);
				if (obj1 == object) return i;

				if (obj1 && object)
				{
					if (*obj1 == *object)
					{
						return i;
					}
				}
				else
				{
					return -1;
				}
			}

			return -1;
		}
	}
}
