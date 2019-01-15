#pragma once

#include "datatype.h"

#include <z/encoding.h>
#include <z/core/sizable.h>

#include <z/core/string.h>
#include <z/core/array.h>

#include <complex>

namespace z
{
	namespace util
	{
		/**
		 * \interface generic
		 * \brief A base object for generic datatypes.
		 *
		 * A wrapper to allow abstraction from primitive data types.
		 */
		class generic : public core::sizable
		{
		public:

			/**
			 * \brief Destructor
			 *
			 * Deletes any allocated data for this object
			 */
			virtual ~generic();

			/**
			 * \brief Get a boolean representation of this object.
			 *
			 * \return True if this object can evaluate to a non-zero number, false otherwise.
			 */
			virtual bool boolean() const = 0;

			/**
			 * \brief Get the pointer this object holds.
			 *
			 * \return A non-specific reference if this object is a pointer type, NULL otherwise.
			 */
			virtual void* pointer() const;

			/**
			 * \brief Get an integer representation of this object.
			 *
			 * \return 0 if this object cannot be represented by an integer, otherwise gives the integer representation.
			 */
			virtual long long integer() const = 0;

			/**
			 * \brief Get a floating-point representation of this object.
			 *
			 * \return 0 if this object cannot be represented by a floating point value, otherwise gives the float representation.
			 */
			virtual double floating() const = 0;

			/**
			 * \brief Get a complex representation of this object.
			 *
			 * \return 0+0i if this object cannot be represented by a complex number, otherwise gives the complex representation.
			 */
			virtual std::complex<double> complex() const = 0;

			/**
			 * \brief Get a string representation of this object.
			 *
			 * \return A string representation of default string encoding.
			 */
			virtual zstring string() const = 0;

			/**
			 * \brief Get the array this object holds.
			 *
			 * \return The item array contained of this is an array object. Gives an empty array otherwise.
			 */
			virtual core::array<generic*> array() const;

			/**
			 * \brief Get this object's type.
			 *
			 * \return A value representing the object type.
			 */
			virtual datatype type() const = 0;

			/**
			 * \brief Get this object's type as a string.
			 *
			 * \return A string representing this object's type with encoding .
			 */
			virtual zstring typeString() const = 0;

			/**
			 * \brief Get whether this object is a pointer.
			 *
			 * \return True if this is a pointer type, false otherwise.
			 */
			virtual bool isPointer() const;

			/**
			 * \brief Get whether this object is a complex number.
			 *
			 * \return True if this object can be accurately represented as a complex number, false otherwise.
			 */
			virtual bool isComplex() const;

			/**
			 * \brief Get whether this object is a floating-point number.
			 *
			 * \return True if this object can be accurately represented as a floating-point value, false otherwise.
			 */
			virtual bool isFloating() const;

			/**
			 * \brief Get whether this object is an integer.
			 *
			 * \return True if this object can be accurately represented as an integer, false otherwise.
			 */
			virtual bool isInteger() const;

			/**
			 * \brief Get whether this object is a string.
			 *
			 * \return True if this is a string type, false otherwise.
			 */
			virtual bool isString() const;

			/**
			 * \brief Get whether this object is an array.
			 *
			 * \return True if this is an array type, false otherwise.
			 */
			virtual bool isArray() const;

			/**
			 * \brief Get whether this object is any kind of number.
			 *
			 * \return True if this object can be accurately represented as any type of number, false otherwise.
			 */
			virtual bool isArithmetic() const;

			/**
			 * \brief Get whether this object is null.
			 *
			 * \return True if this is a NULL object, an empty string, or an empty array. Gives false otherwise.
			 */
			virtual bool isNull() const;

			/**
			 * \brief Get the number of elements this object contains.
			 *
			 * \return If an array, gives the element count. If a string, gives the character count. Gives 0 otherwise.
			 */
			virtual size_t length() const;

			/**
			 * \brief Search the object for an item.
			 *
			 * If this object is a string, then represent element as a string and we search for that substring.
			 * If this object is an array, the search for an item of exactly that type and containing exactly that content (only searches the first level of the array).
			 * Every other type will return -1 as they contain no elements.
			 *
			 * \param element The element to search for.
			 *
			 * \return If the item was found, gives the index where it was found. Otherwise, gives -1.
			 */
			virtual int find(generic* element) const;

			/**
			 * brief Append an element to the object.
			 *
			 * If this object is a string type, the element is cast to a string and the result is appended to the end of this string.
			 * If this object is an array type, then the element is copied and appended to the end of the array.
			 * For all other types, this operation fails.
			 *
			 * \param element The element to append.
			 *
			 * \return True if add succeeded, false if failed.
			 */
			virtual bool add(generic* element);

			/**
			 * brief Insert an element into this object.
			 *
			 * If this object is a string type, the element is cast to a string and the result is inserted before the given character in the string.
			 * If this object is an array type, then the element is copied and inserted before the given index in the array.
			 * For all other types, this operation fails.
			 *
			 * \param element The element to insert.
			 * \param index The index that the element is inserted before.
			 *
			 * \return True if insert succeeded, false if failed.
			 */
			virtual bool insert(generic* element, size_t index);

			/**
			 * brief Remove elements from this object.
			 *
			 * If this object is a string type, then characters are removed from the string.
			 * If this object is an array type, then elements are deleted from the array.
			 * For all other types, this operation fails.
			 *
			 * \param index The first element to remove.
			 * \param count The number of elements to remove. If negative, then elements are removed backwards. If 0, no elements are removed.
			 *
			 * \return True if remove succeeded, false if failed.
			 */
			virtual bool remove(size_t index, int count);

			/**
			 * \brief Create an exact copy of this object.
			 *
			 * Note that this method allocates new data for the duplicate object, so not deleting properly may lead to memory leaks or corruption.
			 *
			 * \return A duplicate of this object, that is, an object of the same type and containing the same data.
			 */
			virtual generic* duplicate() const = 0;

			/**
			 * \brief Equality operator.
			 *
			 * Added for compatibility with the core::array class.
			 *
			 * \param other The object to compare against.
			 *
			 * \return True if both objects are of exactly the same type and contain the same data, false otherwise.
			 */
			virtual bool operator==(const generic& other) const;

			virtual size_t size() const;
		};
	}
}
