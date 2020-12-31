#pragma once

#if __cplusplus < 201703L
#pragma message ("\nNo std::variant available so z::util::generic is disabled.\nCompile with STD=c++17 to enable this feature.")
#else //Otherwise std::variant is available

#include "../core/string.hpp"
#include "../core/array.hpp"
#include <variant>

namespace z
{
	namespace util
	{
		/**
		 * \brief A class for simple storage and manipulation of data regardless of type.
		 *
		 * The idea of this class is to allow somewhat loose typing in c++ projects.
		 * Note that due to type checking, using this class will not be quite as fast as using the raw types.
		 * An example use case is when the data types may be unknown until runtime.
		 */
		class generic
		{
		public:
			///typedef for an array of generic objects
			typedef z::core::array<generic> list;

		private:
			std::variant<bool,long,double,std::complex<double>,zstring,list> value;

		public:
			///List of possible data types
			enum
			{
				VOID,
				INT,
				FLOAT,
				COMPLEX,
				STRING,
				ARRAY
			};

			///Construct as void type.
			generic() noexcept {}

			/**
			 * \brief Construct as integer type.
			 * \param initVal The value to initialize with.
			 */
			template <typename INT, typename = typename std::enable_if<std::is_integral<INT>::value,INT>::type>
			generic(INT initVal) noexcept : value(long(initVal)) {}

			/**
			 * \brief Construct as floating point type.
			 * \param initVal The value to initialize with.
			 */
			generic(double initVal) noexcept : value(initVal) {}

			/**
			 * \brief Construct as complex type.
			 * \param initVal The value to initialize with.
			 */
			generic(const std::complex<double>& initVal) noexcept : value(initVal) {}

			/**
			 * \brief Construct string type from string literal.
			 * \param initVal The value to initialize with.
			 */
			generic(const char* initVal) noexcept : value(zstring(initVal)) {}

			/**
			 * \brief Construct string type from wide string literal.
			 * \param initVal The value to initialize with.
			 */
			generic(const wchar_t* initVal) noexcept : value(zstring(initVal)) {}

			/**
			 * \brief Construct as string type.
			 * \param initVal The value to initialize with.
			 */
			generic(const zstring& initVal) noexcept : value(initVal) {}

			/**
			 * \brief Construct as array type.
			 * \param initVal The value to initialize with.
			 */
			generic(const list& initVal) noexcept : value(initVal) {}

			/**
			 * \brief Explicit conversion to integer types.
			 * \return The contained value, cast to an integer.
			 */
			template <typename INT, typename = typename std::enable_if<std::is_integral<INT>::value,INT>::type>
			explicit inline operator INT() const { return integer(); }

			/**
			 * \brief Explicit conversion to float.
			 * \return The contained value, cast to a float.
			 */
			explicit inline operator float() const noexcept { return floating(); }

			/**
			 * \brief Explicit conversion to double.
			 * \return The contained value, cast to a double.
			 */
			explicit inline operator double() const noexcept { return floating(); }

			/**
			 * \brief Conversion to integer.
			 * \return The contained value, cast to an integer.
			 */
			long integer() const noexcept;

			/**
			 * \brief Conversion to floating point.
			 * \return The contained value, cast to a double.
			 */
			double floating() const noexcept;

			/**
			 * \brief Conversion to a complex value.
			 * \return The contained value, cast to std::complex<double>.
			 */
			std::complex<double> complex() const noexcept;

			/**
			 * \brief Safely convert to a string.
			 *
			 * Generates a string representation of the contained value.
			 * Note that for performance reasons, arrays come back as null strings by default.
			 *
			 * \param printArrays If true, generates a full string representation of array contents.
			 * Otherwise arrays always result in a null string.
			 *
			 * \return A string containing a representation of this value.
			 */
			zstring toString(bool printArrays = false) const noexcept; //safely cast to string; slow, copies to new string

			/**
			 * \brief Get a reference to this string.
			 *
			 * This method is faster than toString() and allows manipulation of string contents.
			 *
			 * \throws std::exception if the current type is not a string.
			 * Use toString() if you do not know for sure what type this is.
			 *
			 * \return A reference to the current string value.
			 */
			zstring& string();

			/**
			 * \brief Get a reference to this array.
			 *
			 * This method allows fast access to and manipulation of array contents.
			 *
			 * \throws std::exception if the current type is not an array.
			 * It's a good idea to check before calling this method.
			 *
			 * \return A reference to the current array value.
			 */
			list& array();

			/**
			 * \brief Whether this value is a numeric type.
			 * \return true if for integer, floating point and complex types, false otherwise.
			 */
			bool numeric() const noexcept;

			/**
			 * \brief Check if this value is a specific type.
			 * \param type The type to check for.
			 * \return true if the types are the same, false otherwise.
			 */
			bool is(std::size_t type) const noexcept;

			/**
			 * \brief Get the current type of this object.
			 * \return The current datatype.
			 */
			int type() const noexcept;

			/**
			 * \brief Get the lowest type this value can be cast to without losing precision.
			 * \param castStrings whether to attempt to downcast strings to numeric types.
			 * \return The lowest possible type this can be cast to without losing precision.
			 */
			int reducesTo(bool castStrings = false) const noexcept; //the lowest we can downcast without losing precision

			/**
			 * \brief Downcast a value as low as possible without losing precision.
			 * \param castStrings Whether to downcast strings to numeric types.
			 * \return true if downcast was successful or resultant types are the same, false otherwise.
			 */
			bool reduce(bool castStrings = false) noexcept;

			/**
			 * \brief Upcast two values as much as is needed for them to be the same type.
			 * \param other The value to upcast along with this.
			 * \return true if upcast was successful or resultant types are the same, false otherwise.
			 */
			bool promote(generic* other) noexcept; //promote two numeric values to the same type

			/**
			 * \brief Add a value to this numeric value.
			 * \throws nonNumeric if the operation is attempted with either value being non-numeric.
			 * \param other The value to add.
			 * \return A reference to this after addition.
			 */
			generic& operator+=(const generic& other);

			/**
			 * \brief Add a value to this numeric value.
			 * \throws nonNumeric if the operation is attempted with either value being non-numeric.
			 * \param other The value to add.
			 * \return The addition result.
			 */
			generic operator+(const generic& other) const { auto tmp = *this; return tmp+=other; }

			/**
			 * \brief Subtract a value from this numeric value.
			 * \throws nonNumeric if the operation is attempted with either value being non-numeric.
			 * \param other The value to subtract.
			 * \return A reference to this after subtraction.
			 */
			generic& operator-=(const generic& other);

			/**
			 * \brief Subtract a value from this numeric value.
			 * \throws nonNumeric if the operation is attempted with either value being non-numeric.
			 * \param other The value to subtract.
			 * \return The subtraction result.
			 */
			generic operator-(const generic& other) const { auto tmp = *this; return tmp-=other; }

			/**
			 * \brief Multiply a value with this numeric value.
			 * \throws nonNumeric if the operation is attempted with either value being non-numeric.
			 * \param other The value to multiply.
			 * \return A reference to this after multiplication.
			 */
			generic& operator*=(const generic& other);

			/**
			 * \brief Multiply a value with this numeric value.
			 * \throws nonNumeric if the operation is attempted with either value being non-numeric.
			 * \param other The value to multiply.
			 * \return The multiplication result.
			 */
			generic operator*(const generic& other) const { auto tmp = *this; return tmp*=other; }

			/**
			 * \brief Divide this value by another numeric value.
			 * \throws nonNumeric if the operation is attempted with either value being non-numeric.
			 * \param other The value to divide by.
			 * \return A reference to this after division.
			 */
			generic& operator/=(const generic& other);

			/**
			 * \brief Divide this value by another numeric value.
			 * \throws nonNumeric if the operation is attempted with either value being non-numeric.
			 * \param other The value to divide by.
			 * \return The division result.
			 */
			generic operator/(const generic& other) const { auto tmp = *this; return tmp/=other; }

			/**
			 * \brief Get the remainder of division by another value.
			 * \throws nonNumeric if the operation is attempted with either value being non-numeric.
			 * \note This casts both values to integers, truncating any other data.
			 * \param other The value to get the remainder of division by.
			 * \return A reference to this after division.
			 */
			generic& operator%=(const generic& other);

			/**
			 * \brief Get the remainder of division by another value.
			 * \throws nonNumeric if the operation is attempted with either value being non-numeric.
			 * \note This casts both values to integers, truncating any other data.
			 * \param other The value to get the remainder of division by.
			 * \return The remainder of division.
			 */
			generic operator%(const generic& other) const { auto tmp = *this; return tmp%=other; }

			/**
			 * \brief Get the negation of this value.
			 * \throws nonNumeric if the operation is attempted on a non-numeric value.
			 * \return A reference to this after negation.
			 */
			generic operator-() const;
		};

	}
}

#endif //End if std::variant is available
