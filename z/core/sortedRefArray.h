#pragma once
#ifndef SORTEDREFARRAY_H_INCLUDED
#define SORTEDREFARRAY_H_INCLUDED

#include "refArray.h"

namespace z
{
	namespace core
	{
		/**
		 * \brief An extension of the core::refArray class which
		 * attempts to keep all data sorted.
		 *
		 * Unlike with the <B>sorted array</B>, elements are
		 * assumed to be pointers and are sorted
		 * with this in mind.
		 *
		 * \see array
		 * \see refArray
		 * \see sortedArray
		 */
		template <typename T>
		class sortedRefArray : public refArray<T>
		{
		private:
			inline void init(const T arg1)
			{
				add(arg1);
			}

			template <typename... Args>
			inline void init(const T arg1, const Args... args)
			{
				add(arg1);

				init(args...);
			}

		public:
			///Default constructor
			sortedRefArray() {}

			sortedRefArray(const refArray<T>&);
			sortedRefArray(const T);

			template <typename... Args>
			sortedRefArray(const T arg1, const Args... args);

			virtual size_t add(const T&);
			virtual void add(const refArray<T>&);

			intmax_t find(const T&) const;
			intmax_t findInsert(const T&) const;

			virtual void sort();
		};

		///Copy constructor
		template <typename T>
		sortedRefArray<T>::sortedRefArray(const refArray<T>& other)
		{
			for (size_t i=0; i<other.length(); i++)
				add(other[i]);
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
		sortedRefArray<T>::sortedRefArray(const T arg1)
		{
			this->array_data.push_back(arg1);
		}


		/**
		 * \brief List-initialized constructor.
		 *
		 * Constructs the array with an arbitrary
		 * number of elements already contained and
		 * sorted.<BR>
		 *
		 * \b Syntax: array<T> X {arg1, arg2, ...};
		 * array<T> X = {arg1, arg2, ...};
		 *
		 * \param arg1 initializing data.
		 * \param args cont. initializing data.
		 */
		template <typename T>
		template <typename... Args>
		sortedRefArray<T>::sortedRefArray(const T arg1, const Args... args)
		{
			init(arg1, args...);
		}


		/**
		 * \brief Add an object pointer to the array.
		 *
		 * Finds an appropriate location and inserts the
		 * pointer into the array, such that the array
		 * remains sorted.
		 *
		 * \param object the pointer to add to the array.
		 *
		 * \return The index where the inserted pointer now resides.
		 */
		template <typename T>
		size_t sortedRefArray<T>::add(const T& object)
		{
			size_t index = findInsert(object);

			this->insert(object, index);

			return index;
		}

		/**
		 * \brief Add the contents of another array to this one.
		 *
		 * For each of the elements in the other array,
		 * this function finds an appropriate location in this
		 * array and inserts the element, such that this array
		 * remains sorted.
		 *
		 * \param other the array to copy from.
		 */
		template <typename T>
		void sortedRefArray<T>::add(const refArray<T>& other)
		{
			for (size_t i=0; i<other.length(); i++)
			{
				int index = findInsert(other[i]);

				this->insert(other[i], index);
			}
		}

		/**
		 * \brief Check if a given object with identical
		 * data is in the array.
		 *
		 * Locates the desired index using a binary search,
		 * as the array is expected to be sorted.
		 *
		 * \param object a pointer to the object to search for.
		 *
		 * \return The first index that the object data was found at.
		 * \b -1 if it was not found.
		 */
		template <typename T>
		intmax_t sortedRefArray<T>::find(const T& object) const
		{
			if (this->array_data.size() == 0)
				return -1;

			intmax_t left = 0;
			intmax_t right = this->array_data.size()-1;

			while (left < right)
			{
				int center = (left + right) / 2;

				if (*(this->array_data.at(center)) < *object)
				{
					left = center + 1;
				}
				else if (*(this->array_data.at(center)) > *object)
				{
					right = center - 1;
				}
				else
				{
					return center;
				}
			}

			if (*(this->array_data.at(left)) == *object)
				return left;
			else
				return -1;
		}

		/**
		 * \brief Find an index where the given object pointer
		 * can be inserted while keeping the array sorted.
		 *
		 * Locates the desired index using a binary search,
		 * as the array is expected to be sorted.
		 *
		 * \param object pointer to the object to search for.
		 *
		 * \return The first index where the pointer can be
		 * inserted.
		 */
		template <typename T>
		intmax_t sortedRefArray<T>::findInsert(const T& object) const
		{
			if (this->array_data.size() == 0)
				return 0;

			intmax_t left = 0;
			intmax_t right = this->array_data.size()-1;

			while (left < right)
			{
				int center = (left + right) / 2;

				if (*(this->array_data.at(center)) < *object)
				{
					left = center + 1;
				}
				else if (*(this->array_data.at(center)) > *object)
				{
					right = center - 1;
				}
				else
				{
					return center;
				}
			}

			if (*(this->array_data.at(left)) < *object)
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
		void sortedRefArray<T>::sort()
		{
			bool done = true;

			do
			{
				done = true;


				for (int i=0; i<(int)this->array_data.size()-1; i++)
				{
					if (*(this->array_data[i]) > *(this->array_data[i+1]))
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

#endif // SORTEDREFARRAY_H_INCLUDED
