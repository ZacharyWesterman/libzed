#pragma once

#include <vector>
#include <initializer_list>
#include "stream.hpp"
#include "sizable.hpp"
#include "typeChecks.hpp"
#include "compare.hpp"

#ifdef __has_include
#if __has_include(<cereal/cereal.hpp>)
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#endif
#endif

namespace z
{
	namespace core
	{
		/**
		* \brief A wrapper for std::vector.
		*
		* This class is a wrapper for the std::vector class
		* that adds ease of use and hides implementation from the user.
		*
		* /note This will only compile for objects that are copyable.
		* Use std::vector for non-copyable objects.
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

			/**
			* \brief Helper function for single object initialization.
			* \param arg1 The object to initialize the array with.
			*/
			inline void init(const T& arg1)
			{
				add(arg1);
			}

			/**
			* \brief Helper function for brace-enclosed list initialization.
			* \param arg1 The first object to add to the array.
			* \param args Any following objects to add to the array.
			*/
			template <typename... Args>
			inline void init(const T& arg1, const Args&... args)
			{
				add(arg1);

				init(args...);
			}

			/**
			* \brief Check if two objects are equal.
			*
			* This member function allows object comparison to be
			* different for different array types (e.g. reference arrays
			* will need to sort by value after dereference).
			*
			* \param arg1 First object to compare.
			* \param arg2 Second object to compare.
			*
			* \return True if the objects are equal, false otherwise.
			*/
			virtual bool eq(const T& arg1, const T& arg2) const
			{
				return equals(arg1, arg2);
			}

			/**
			* \brief Check if one object is greater than another.
			*
			* This member function allows object comparison to be
			* different for different array types (e.g. reference arrays
			* will need to sort by value after dereference).
			*
			* \param arg1 First object to compare.
			* \param arg2 Second object to compare.
			*
			* \return True if arg1 is greater than arg2, false otherwise.
			*/
			virtual bool gt(const T& arg1, const T& arg2) const
			{
				return greater(arg1, arg2);
			}

			/**
			* \brief Check if one object is less than another.
			*
			* This member function allows object comparison to be
			* different for different array types (e.g. reference arrays
			* will need to sort by value after dereference).
			*
			* \param arg1 First object to compare.
			* \param arg2 Second object to compare.
			*
			* \return True if arg1 is less than arg2, false otherwise.
			*/
			virtual bool lt(const T& arg1, const T& arg2) const
			{
				return lesser(arg1, arg2);
			}

		public:
			///Default constructor.
			array() {}

			array(const array&);

			template <typename... Args>
			array(const T& arg1, const Args&... args);

			/**
			* \brief Construct from a generic initializer list
			*
			* \param other The list to initialize this array with.
			*/
			array(const std::initializer_list<T>& other) :
				array_data(other)
			{}

			///Destructor
			virtual ~array() {}

			inline void clear();

			/**
			* \brief Increase the space allocated for this array.
			*
			* If this array currently has fewer than newSize elements allocated,
			* enough space is reallocated to hold at least that many characters.
			*
			* \param newSize The minimum number of elements this array should be able to contain.
			*/
			void increase(int newSize) noexcept
			{
				array_data.reserve(newSize);
			}

			/**
			* \brief Add an object to the array.
			*
			* Adds the given data to a position in the array. That
			* position is up to implementation.
			*
			* \param object the data to add to the array.
			*
			* \return The index where the inserted object now resides.
			*/
			virtual int add(const T& object)
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
			void add(const array& other)
			{
				for (int i=0; i<other.length(); i++)
					add(other.array_data[i]);
			}

			array& insert(const T&, int);

			void append(const T&);

			array& remove(int);
			array& remove(int, int);

			array& replace(int, int, const T&);
			array& replace(int, int, const array<T>&);


			array subset(int, int) const;

			size_t size() const noexcept;

			int length() const;

			T& at(int);
			const T& at(int) const;
			T& operator[](int);
			const T& operator[](int) const;

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
			virtual int find(const T& object) const
			{
				for (int i=0; i<(int)array_data.size(); i++)
					if (eq(array_data.at(i),object))
						return i;

				return -1;
			}

			array& operator=(const array& other);
			array& operator=(const std::initializer_list<T>& other);

			bool operator==(const array& other) const;
			bool operator>(const array& other) const;
			bool operator<(const array& other) const;
			inline bool operator>=(const array& other) const;
			inline bool operator<=(const array& other) const;


			bool isValid(int position) const;

			/**
			* \brief Get pointer to the beginning of the array.
			*
			* This member function should not be used directly.
			* It is meant for C++11's range-based for loop syntax.
			*
			* \return A pointer to the first element in the array. 0 if no elements.
			*/
			T* begin() const
			{
				return array_data.empty() ? NULL : (T*)&array_data.front();
			}

			/**
			* \brief Get pointer to the end of the array.
			*
			* This member function should not be used directly.
			* It is meant for C++11's range-based for loop syntax.
			*
			* \return A pointer to right after the last element in the array. 0 if no elements.
			*/
			T* end() const
			{
				return begin() + array_data.size();
			}

#		ifdef __has_include
#		if __has_include(<cereal/cereal.hpp>)
			/**
			* \brief JSON specialization of serialization output.
			* \param ar The output archive.
			*/
			void save(cereal::JSONOutputArchive& ar) const
			{
				ar.makeArray();
				for (int i=0; i<(int)array_data.size(); i++)
				{
					ar(array_data[i]);
				}
			}

			/**
			* \brief XML specialization of serialization output.
			* \param ar The output archive.
			*/
			void save(cereal::XMLOutputArchive& ar) const
			{
				for (int i=0; i<(int)array_data.size(); i++)
				{
					ar(cereal::make_nvp(std::to_string(i),array_data[i]));
				}
			}

			/**
			* \brief Binary specialization of serialization output.
			* \param ar The output archive.
			*/
			template <typename archive>
			void save(archive& ar) const
			{
				ar((size_t)array_data.size());
				for (int i=0; i<(int)array_data.size(); i++)
				{
					ar(array_data[i]);
				}
			}

			/**
			* \brief JSON specialization of serialization input.
			* \param ar The input archive.
			*/
			void load(cereal::JSONInputArchive& ar)
			{
				CEREAL_SIZE_TYPE sz;
				ar.loadSize(sz);
				array_data.reserve(sz);

				T data;
				for (int i=0; i<sz; i++)
				{
					ar(data);
					array_data.push_back(data);
				}
			}

			/**
			* \brief XML specialization of serialization input.
			* \param ar The input archive.
			*/
			void load(cereal::XMLInputArchive& ar)
			{
				CEREAL_SIZE_TYPE sz;
				ar.loadSize(sz);
				array_data.reserve(sz);

				T data;
				for (CEREAL_SIZE_TYPE i=0; i<sz; i++)
				{
					ar(data);
					array_data.push_back(data);
				}
			}

			/**
			* \brief Binary specialization of serialization input.
			* \param ar The input archive.
			*/
			template <class archive>
			void load(archive& ar)
			{
				clear();
				CEREAL_SIZE_TYPE sz;
				ar(sz);
				array_data.reserve(sz);

				T data;
				for (CEREAL_SIZE_TYPE i=0; i<sz; i++)
				{
					ar(data);
					array_data.push_back(data);
				}
			}

#		endif
#		endif
		};


		///Copy constructor
		template <typename T>
		array<T>::array(const array<T>& other)
		{
			array_data = other.array_data;
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
		array<T>& array<T>::operator=(const array<T>& other)
		{
			array_data = other.array_data;

			return *this;
		}

		/**
		* \brief Initializer list assignment operator.
		*
		* Clear the contents of this array and copy the contents
		* of an initializer list into this array.
		*
		* \param other The initializer list to copy from.
		*
		* \return This array after the operation (for
		* \b a=b=c type expressions).
		*/
		template <typename T>
		array<T>& array<T>::operator=(const std::initializer_list<T>& other)
		{
			array_data.clear();
			array_data.reserve(other.size());
			for (auto& item : other) array_data.push_back(item);

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

			for (int i=0; i<(int)array_data.size(); i++)
				if (!eq(array_data.at(i),other.array_data.at(i)))
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

			for (int i=0; i<(int)array_data.size(); i++)
			{
				if (gt(array_data.at(i),other.array_data.at(i)))
					gt_count++;
				else if (lt(array_data.at(i),other.array_data.at(i)))
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

			for (int i=0; i<(int)array_data.size(); i++)
			{
				if (gt(array_data.at(i),other.array_data.at(i)))
					gt_count++;
				else if (lt(array_data.at(i),other.array_data.at(i)))
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


		///Clear the data in the array.
		template <typename T>
		inline void array<T>::clear()
		{
			array_data.clear();
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
		array<T>& array<T>::insert(const T& object, int index)
		{
			//if index is negative, insert from end of the array.
			if (index < 0) index += array_data.size() + 1;

			//keep within bounds of array.
			if (index > (int)array_data.size()) index = array_data.size();
			if (index < 0) index = 0;

			array_data.insert(array_data.begin() + index, object);

			return *this;
		}

		/**
		* \brief Append an object to the end of the array.
		*
		* Appends the given data to the end of the array.
		*
		* \param object the data to aappend to the array.
		*/
		template <typename T>
		void array<T>::append(const T& object)
		{
			array_data.push_back(object);
		}

		/**
		* \brief Remove an object from the array.
		*
		* \param index the index of the object to be removed.
		*
		* \return A reference to this array after modification.
		*/
		template <typename T>
		array<T>& array<T>::remove(int index)
		{
			if (index < 0) index += array_data.size() + 1;
			if ((index >= array_data.size()) || (index < 0)) return *this;

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
		array<T>& array<T>::remove(int index, int count)
		{
			if (!count) return *this;

			if (index < 0) index += array_data.size() + 1;

			int start, end;

			if (count > 0)
			{
				start = index;
				end = index + count;
			}
			else
			{
				start = index + count + 1;
				end = index + 1;
			}

			if ((end <= 0) || (start >= (int)array_data.size())) return *this;
			if (start < 0) start = 0;
			if (end > (int)array_data.size()) end = array_data.size();

			array_data.erase(array_data.begin() + start, array_data.begin() + end);

			return *this;
		}


		/**
		* \brief Get the size of the array.
		*
		* \return The (approximate) number of bytes the array consumes.
		*/
		template <typename T>
		size_t array<T>::size() const noexcept
		{
			size_t bytes = 0;
			for (auto& item : array_data)
			{
				size_t objBytes;
				z::core::size(item, objBytes);
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
		int array<T>::length() const
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
		* \see operator[](int)
		*/
		template <typename T>
		T& array<T>::at(int index)
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
		* \see operator[](int) const
		*/
		template <typename T>
		const T& array<T>::at(int index) const
		{
			return array_data.at(index);
		}

		/**
		* \brief Function to get the object at the given index.
		*
		* Identical behavior to at(int), but allows indexing
		* with square brackets.
		*
		* \param index the index of the desired object.
		*
		* \return The object at the given index.
		*
		* \see at(int)
		*/
		template <typename T>
		T& array<T>::operator[](int index)
		{
			return array_data.at(index);
		}

		/**
		* \brief Const function to get the object at the given index.
		*
		* Identical behavior to at(int), but allows indexing
		* with square brackets.
		*
		* \param index the index of the desired object.
		*
		* \return The object at the given index.
		*
		* \see at(int) const
		*/
		template <typename T>
		const T& array<T>::operator[](int index) const
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
		* \see replace(int,int,const array&)
		*/
		template <typename T>
		array<T>& array<T>::replace(int index, int count, const T& object)
		{
			if (!count) return *this;

			if (index < 0) index += array_data.size() + 1;

			int start, end;

			if (count > 0)
			{
				start = index;
				end = index + count;
			}
			else
			{
				start = index + count + 1;
				end = index + 1;
			}

			if ((end <= 0) || (start >= (int)array_data.size())) return *this;
			if (start < 0) start = 0;
			if (end > (int)array_data.size()) end = array_data.size();

			array_data.erase(array_data.begin() + start, array_data.begin() + end);
			array_data.insert(array_data.begin() + start, object);

			return *this;
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
		* \see replace(int,int,const T&)
		*/
		template <typename T>
		array<T>& array<T>::replace(int index, int count, const array<T>& other)
		{
			if (index < 0) index += array_data.size();

			int start, end;

			if (count >= 0)
			{
				start = index;
				end = index + count;
			}
			else
			{
				start = index + count + 1;
				end = index + 1;
			}

			if ((end <= 0) || (start >= (int)array_data.size())) return *this;
			if (start < 0) start = 0;
			if (end > (int)array_data.size()) end = array_data.size();

			if (count) array_data.erase(array_data.begin() + start, array_data.begin() + end);
			array_data.insert(array_data.begin() + start, other.array_data.begin(), other.array_data.end());

			return *this;
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
		array<T> array<T>::subset(int index, int count) const
		{
			array<T> output;

			if (!count) return *this;

			if (index < 0) index += array_data.size() + 1;

			int start, end;

			if (count > 0)
			{
				start = index;
				end = index + count;
			}
			else
			{
				start = index + count + 1;
				end = index + 1;
			}

			if ((end <= 0) || (start >= (int)array_data.size())) return *this;
			if (start < 0) start = 0;
			if (end > (int)array_data.size()) end = array_data.size();

			if (end-start > 0) output.array_data.reserve(end-start);
			for (int i=start; i<end; i++)
				output.array_data.push_back(array_data[i]);

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
		bool array<T>::isValid(int index) const
		{
			if (index < 0) index += array_data.size();
			return (index < (int)array_data.size()) && (index >= 0);
		}
	}
}
