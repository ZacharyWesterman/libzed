#pragma once
#ifndef SORTEDARRAY_H_INCLUDED
#define SORTEDARRAY_H_INCLUDED

#include "array.hpp"

namespace z
{
	namespace core
	{
		/**
		 * \brief An extension of the core::array class
		 * which attempts to keep all data sorted.
		 *
		 * \see array
		 * \see refArray
		 * \see sortedRefArray
		 */
		template <typename T>
		class sortedArray : public array<T>
		{
		public:
			sortedArray() {}

			template <typename... Args>
			sortedArray(const T& arg1, const Args&... args)
			{
				this->init(arg1, args...);
			}

			virtual size_t add(const T&) override;

			virtual intmax_t find(const T&) const override;
			virtual intmax_t findInsert(const T&, bool allowDuplicates = true) const;

			virtual void sort();
		};

		/**
		 * \brief Add an object to the array.
		 *
		 * Finds an appropriate location and inserts the
		 * object into the array, such that the array
		 * remains sorted.
		 *
		 * \param object the data to add to the array.
		 *
		 * \return The index where the inserted object now resides.
		 */
		template <typename T>
		size_t sortedArray<T>::add(const T& object)
		{
			size_t index = findInsert(object);

			this->insert(object, index);

			return index;
		}

		/**
		 * \brief Check if a given object is in the array.
		 *
		 * Locates the desired index using a binary search,
		 * as the array is expected to be sorted.
		 *
		 * /note The objects this array contains must be trivially
		 * comparable, or have operator>() and operator<() defined.
		 *
		 * \param object the object to search for.
		 *
		 * \return The first index that the object was found at.
		 * \b -1 if it was not found.
		 */
		template <typename T>
		intmax_t sortedArray<T>::find(const T& object) const
		{
			if (this->array_data.size() == 0)
				return -1;

			intmax_t left = 0;
			intmax_t right = this->array_data.size()-1;

			while (left < right)
			{
				intmax_t center = (left + right) / 2;

				if (this->lt(this->array_data.at(center), object))
				{
					left = center + 1;
				}
				else if (this->gt(this->array_data.at(center), object))
				{
					right = center - 1;
				}
				else
				{
					return center;
				}
			}

			if (this->eq(this->array_data.at(left), object))
				return left;
			else
				return -1;
		}

		/**
		 * \brief Find an index where the given object
		 * can be inserted while keeping the array sorted.
		 *
		 * Locates the desired index using a binary search,
		 * as the array is expected to be sorted.
		 *
		 * \param object the object to search for.
		 *
		 * \return The first index where the object can be
		 * inserted.
		 */
		template <typename T>
		intmax_t sortedArray<T>::findInsert(const T& object, bool allowDuplicates) const
		{
			if (this->array_data.size() == 0)
				return 0;

			intmax_t left = 0;
			intmax_t right = this->array_data.size()-1;

			while (left < right)
			{
				intmax_t center = (left + right) / 2;

				if (this->lt(this->array_data.at(center), object))
				{
					left = center + 1;
				}
				else if (this->gt(this->array_data.at(center), object))
				{
					right = center - 1;
				}
				else
				{
					return center;
				}
			}

			if (this->lt(this->array_data.at(left), object))
				return left+1;
			else
				return left;
		}

		/**
		 * \brief Sorts the array.
		 *
		 * This function currently uses bubble-sort. As
		 * such, it is <B>very slow</B> for large arrays!
		 * I intend to implement a faster version later.
		 */
		template <typename T>
		void sortedArray<T>::sort()
		{
			bool done = true;

			do
			{
				done = true;


				for (size_t i=0; i<(size_t)this->array_data.size()-1; i++)
				{
					if (this->gt(this->array_data[i],this->array_data[i+1]))
					{
						done = false;

						T temp = this->array_data[i];
						this->array_data[i] = this->array_data[i+1];
						this->array_data[i+1] = temp;
					}
				}
			}
			while (!done);
		}
	}
}

#endif // SORTEDARRAY_H_INCLUDED
