#pragma once

#include <vector>
#include "stream.h"
#include "sizable.h"

namespace z
{
	namespace core
	{
		/**
		 * \brief A wrapper for std::vector.
		 *
		 * This class is a wrapper for the std::vector class
		 * that adds ease of use.<BR>Additionally, if I ever decide to
		 * stop using std::vector, I won't have to change all of my code,
		 * just this class.
		 * <br/><br/>
		 * <B>RE-ENTRANCE:</B><br/>
		 * Simultaneous accesses to the same object can cause data races.
		 *
		 * \see refArray
		 * \see sortedArray
		 * \see sortedRefArray
		 */
		template <typename T>
		class array : public sizable
		{
		protected:
			///The data in the array.
			std::vector<T> array_data;

		private:
			inline void init(const T& arg1)
			{
				array_data.push_back(arg1);
			}

			template <typename... Args>
			inline void init(const T& arg1, const Args&... args)
			{
				array_data.push_back(arg1);

				init(args...);
			}

		public:
			///Default constructor.
			array() {}

			array(const array&);
			array(const T&);

			template <typename... Args>
			array(const T& arg1, const Args&... args);

			///Destructor
			virtual ~array() {}

			inline void clear();

			virtual size_t add(const T&);
			virtual void add(const array&);
			const array& insert(const T&, size_t);
			const array& remove(size_t);
			const array& remove(size_t, int);

			const array& replace(size_t, int, const T&);
			const array& replace(size_t, int, const array<T>&);


			array subset(size_t, int) const;

			size_t size() const;

			size_t length() const;

			inline T& at(size_t);
			inline const T& at(size_t) const;
			inline T& operator[](size_t);
			inline const T& operator[](size_t) const;


			virtual int find(const T& object) const;

			const array& operator=(const array& other);

			bool operator==(const array& other) const;
			bool operator>(const array& other) const;
			bool operator<(const array& other) const;
			inline bool operator>=(const array& other) const;
			inline bool operator<=(const array& other) const;


			bool isValid(size_t position) const;
		};


		///Copy constructor
		template <typename T>
		array<T>::array(const array<T>& other)
		{
			array_data = other.array_data;
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
		array<T>::array(const T& arg1)
		{
			array_data.push_back(arg1);
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
		array<T>::array(const T& arg1, const Args&... args)
		{
			init(arg1, args...);
		}

		/**
		 * \brief Array assignment operator.
		 *
		 * Clear the contents of this array and create
		 * a copy of another array's contents into this one.
		 *
		 * \param other the array to copy from.
		 *
		 * \return This array after the operation (for
		 * \b a=b=c type expressions).
		 */
		template <typename T>
		const array<T>& array<T>::operator=(const array<T>& other)
		{
			array_data = other.array_data;

			return *this;
		}

		/**
		 * \brief Check whether two arrays' contents are the same.
		 *
		 * \param other the array to compare with this one.
		 *
		 * \return \b True if both arrays contain the same
		 * contents in the same order, and the same number of
		 * contents. \b False otherwise.
		 */
		template <typename T>
		bool array<T>::operator==(const array<T>& other) const
		{
			if (array_data.size() != other.array_data.size())
				return false;

			for (size_t i=0; i<array_data.size(); i++)
				if (array_data.at(i) != other.array_data.at(i))
					return false;

			return true;
		}

		/**
		 * \brief Array greater-than operator
		 *
		 * \param other the array to compare with this one.
		 *
		 * \return \b True if the difference between the two arrays'
		 * elements add to a positive number, or this array has more
		 * elements. \b False otherwise.
		 */
		template <typename T>
		bool array<T>::operator>(const array<T>& other) const
		{
			if (array_data.size() != other.array_data.size())
				return (array_data.size() > other.array_data.size());

			int gt_count = 0;

			for (size_t i=0; i<array_data.size(); i++)
			{
				if (array_data.at(i) > other.array_data.at(i))
					gt_count++;
				else if (array_data.at(i) < other.array_data.at(i))
					gt_count--;
			}

			return gt_count > 0;
		}

		/**
		 * \brief Array less-than operator
		 *
		 * \param other the array to compare with this one.
		 *
		 * \return \b True if the difference between the two arrays'
		 * elements add to a negative number, or this array has fewer
		 * elements. \b False otherwise.
		 */
		template <typename T>
		bool array<T>::operator<(const array& other) const
		{
			if (array_data.size() != other.array_data.size())
				return (array_data.size() < other.array_data.size());

			int gt_count = 0;

			for (size_t i=0; i<array_data.size(); i++)
			{
				if (array_data.at(i) > other.array_data.at(i))
					gt_count++;
				else if (array_data.at(i) < other.array_data.at(i))
					gt_count--;
			}

			return gt_count < 0;
		}

		/**
		 * \brief Array greater-than-or-equal operator
		 *
		 * \param other the array to compare with this one.
		 *
		 * \return \b False if the difference between the two arrays'
		 * elements add to a negative number, or this array has fewer
		 * elements. \b True otherwise.
		 */
		template <typename T>
		inline bool array<T>::operator>=(const array& other) const
		{
			return !operator<(other);
		}

		/**
		 * \brief Array less-than-or-equal operator
		 *
		 * \param other the array to compare with this one.
		 *
		 * \return \b False if the difference between the two arrays'
		 * elements add to a positive number, or this array has more
		 * elements. \b True otherwise.
		 */
		template <typename T>
		inline bool array<T>::operator<=(const array<T>& other) const
		{
			return !operator>(other);
		}

		/**
		 * \brief Check if a given object is in the array.
		 *
		 * Locates the desired index using a linear search,
		 * as the array is expected to be unsorted.
		 *
		 * \param object the object to search for.
		 *
		 * \return The first index that the object was found at.
		 * \b -1 if it was not found.
		 */
		template <typename T>
		int array<T>::find(const T& object) const
		{
			for (size_t i=0; i<array_data.size(); i++)
				if (array_data.at(i) == object)
					return i;

			return -1;
		}


		///Clear the data in the array.
		template <typename T>
		inline void array<T>::clear()
		{
			array_data.clear();
		}


		/**
		 * \brief Add an object to the array.
		 *
		 * Appends the given data to the end of the array.
		 *
		 * \param object the data to add to the array.
		 *
		 * \return The index where the inserted object now resides.
		 */
		template <typename T>
		size_t array<T>::add(const T& object)
		{
			array_data.push_back(object);

			return (array_data.size() - 1);
		}

		/**
		 * \brief Add another array to this array.
		 *
		 * Copies the contents of another array and
		 * appends them to the end of this array.
		 *
		 * \param other the array to copy from.
		 */
		template <typename T>
		void array<T>::add(const array& other)
		{
			for (size_t i=0; i<other.size(); i++)
				array_data.push_back(other[i]);
		}


		/**
		 * \brief Insert an object into the array.
		 *
		 * Inserts an object into the given index in the array, if possible.
		 *
		 * \param object the data to add to the array.
		 * \param index the index in the array to insert the object.
		 *
		 * \return A reference to this array after modification.
		 */
		template <typename T>
		const array<T>& array<T>::insert(const T& object, size_t index)
		{
			//if invalid index, return false
			if (index > array_data.size())
				return *this;

			array_data.insert(array_data.begin() + index, object);

			return *this;
		}


		/**
		 * \brief Remove an object from the array.
		 *
		 * \param index the index of the object to be removed.
		 *
		 * \return A reference to this array after modification.
		 */
		template <typename T>
		const array<T>& array<T>::remove(size_t index)
		{
			if (index >= array_data.size())
				return *this;

			array_data.erase(array_data.begin() + index);

			return *this;
		}

		/**
		 * \brief Remove all elements in a subset of the array.
		 *
		 * \param index the index of the first object to be removed.
		 * \param count the number of objects to be removed.
		 *
		 * \return A reference to this array after modification.
		 */
		template <typename T>
		const array<T>& array<T>::remove(size_t index, int count)
		{
			if (index >= array_data.size())
				index = array_data.size()-1;

			size_t start, end;

			if (count < 0)
			{
				if ((size_t)-count >= index)
					start = 0;
				else
					start = index + count;

				end = index + 1;
			}
			else if (count)
			{
				end = index + count;
				if (end > array_data.size()) end = array_data.size();

				start = index;
			}
			else return *this;

			array_data.erase(array_data.begin() + start, array_data.begin() + end);

			return *this;
		}


		/**
		 * \brief Get the size of the array.
		 *
		 * \return The (approximate) number of bytes the array consumes.
		 */
		template <typename T>
		size_t array<T>::size() const
		{
			size_t bytes = 0;
			for (size_t i=0; i<array_data.size(); i++)
			{
				size_t objBytes;
				z::core::size(array_data[i], objBytes);
				bytes += objBytes;
			}
			return bytes;
		}

		/**
		 * \brief Get the length of the array.
		 *
		 * \return The number of objects in the array.
		 */
		template <typename T>
		size_t array<T>::length() const
		{
			return array_data.size();
		}

		/**
		 * \brief Function to get the object at the given index.
		 *
		 * \param index the index of the desired object.
		 *
		 * \return The object at the given index.
		 *
		 * \see operator[](size_t)
		 */
		template <typename T>
		inline T& array<T>::at(size_t index)
		{
			return array_data.at(index);
		}

		/**
		 * \brief Const function to get the object at the given index.
		 *
		 * \param index the index of the desired object.
		 *
		 * \return The object at the given index.
		 *
		 * \see operator[](size_t) const
		 */
		template <typename T>
		inline const T& array<T>::at(size_t index) const
		{
		   return array_data.at(index);
		}

		/**
		 * \brief Function to get the object at the given index.
		 *
		 * Identical behavior to at(size_t), but allows indexing
		 * with square brackets.
		 *
		 * \param index the index of the desired object.
		 *
		 * \return The object at the given index.
		 *
		 * \see at(size_t)
		 */
		template <typename T>
		inline T& array<T>::operator[](size_t index)
		{
			return array_data.at(index);
		}

		/**
		 * \brief Const function to get the object at the given index.
		 *
		 * Identical behavior to at(size_t), but allows indexing
		 * with square brackets.
		 *
		 * \param index the index of the desired object.
		 *
		 * \return The object at the given index.
		 *
		 * \see at(size_t) const
		 */
		template <typename T>
		inline const T& array<T>::operator[](size_t index) const
		{
			return array_data.at(index);
		}


		/**
		 * \brief Replace all objects in the given range with an object.
		 *
		 * \param index the index of the first object to replace.
		 * \param count the number of objects to replace.
		 * \param object the object to insert into the gap.
		 *
		 * \return A reference to this array after modification.
		 *
		 * \see replace(size_t,int,const array&)
		 */
		template <typename T>
		const array<T>& array<T>::replace(size_t index, int count, const T& object)
		{
			if (index >= array_data.size())
				index = array_data.size()-1;

			size_t start, end;

			if (count < 0)
			{
				if ((size_t)-count >= index)
					start = 0;
				else
					start = index + count;

				end = index + 1;
			}
			else if (count)
			{
				end = index + count;
				if (end > array_data.size()) end = array_data.size();

				start = index;
			}
			else return;

			array_data.erase(array_data.begin() + start, array_data.begin() + end);
			array_data.insert(array_data.begin() + start, object);
		}

		/**
		 * \brief Replace all objects in the given range with an array of objects.
		 *
		 * \param index the index of the first object to replace.
		 * \param count the number of objects to replace.
		 * \param other the array to copy from.
		 *
		 * \return A reference to this array after modification.
		 *
		 * \see replace(size_t,int,const T&)
		 */
		template <typename T>
		const array<T>& array<T>::replace(size_t index, int count, const array<T>& other)
		{
			if (index >= array_data.size())
				index = array_data.size()-1;

			size_t start, end;

			if (count < 0)
			{
				if ((size_t)-count >= index)
					start = 0;
				else
					start = index + count;

				end = index + 1;
			}
			else if (count)
			{
				end = index + count;
				if (end > array_data.size()) end = array_data.size();

				start = index;
			}
			else return;

			array_data.erase(array_data.begin() + start, array_data.begin() + end);

			for (size_t i=0; i<other.size(); i++)
				array_data.insert(array_data.begin() + start + i, other[i]);
		}


		/**
		 * \brief Get a contiguous subset of the elements in the array.
		 *
		 * Copies all elements in the given range, inclusive. If either
		 * of the parameters is \b -1, gives an empty array. If the
		 * \b stop parameter is less than \b start, then the subset is
		 * copied in reverse order.
		 *
		 * \param index the index of the first object to copy.
		 * \param count the number of objects to copy.
		 *
		 * \return A subset of the main array.
		 */
		template <typename T>
		array<T> array<T>::subset(size_t index, int count) const
		{
			array<T> output;

			if (index >= array_data.size())
				index = array_data.size()-1;

			size_t start, end;

			if (count < 0)
			{
				if ((size_t)-count >= index)
					start = 0;
				else
					start = index + count;

				end = index;

				size_t offset = end+start;
				for (size_t i=start; i<end; i++)
					output.array_data.push_back(array_data[offset-i]);
			}
			else if (count)
			{
				end = index + count;
				if (end > array_data.size()) end = array_data.size();

				start = index;

				for (size_t i=start; i<end; i++)
					output.array_data.push_back(array_data[i]);
			}

			return output;
		}


		/**
		 * \brief Check if an index is within the bounds of the array.
		 *
		 * \param index the index to check.
		 *
		 * \return \b True if the given index is within array bounds.
		 * \b False otherwise.
		 */
		template <typename T>
		bool array<T>::isValid(size_t index) const
		{
			return (index < array_data.size());
		}
	}
}
