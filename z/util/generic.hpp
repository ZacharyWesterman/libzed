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
		class generic
		{
		public:
			typedef z::core::array<generic> list;

		private:
			std::variant<bool,long,double,std::complex<double>,zstring,list> value;

		public:
			enum //allowed data types
			{
				VOID,
				INT,
				FLOAT,
				COMPLEX,
				STRING,
				ARRAY
			};

			generic() noexcept {}

			//initializers
			template <typename INT, typename = typename std::enable_if<std::is_integral<INT>::value,INT>::type>
			generic(INT initVal) noexcept : value(long(initVal)) {}

			generic(float initVal) noexcept : value(double(initVal)) {}
			generic(double initVal) noexcept : value(initVal) {}
			generic(const std::complex<double>& initVal) noexcept : value(initVal) {}
			generic(const char* initVal) noexcept : value(zstring(initVal)) {}
			generic(const wchar_t* initVal) noexcept : value(zstring(initVal)) {}
			generic(const zstring& initVal) noexcept : value(initVal) {}
			generic(const list& initVal) noexcept : value(initVal) {}

			//assignment operators
			template <typename INT, typename = typename std::enable_if<std::is_integral<INT>::value,INT>::type>
			generic& operator=(INT initVal) noexcept { value = long(initVal); return *this; }

			generic& operator=(float initVal) noexcept { value = double(initVal); return *this; }
			generic& operator=(double initVal) noexcept { value = initVal; return *this; }
			generic& operator=(const std::complex<double>& initVal) noexcept { value = initVal; return *this; }
			generic& operator=(const char* initVal) noexcept { value = zstring(initVal); return *this; }
			generic& operator=(const wchar_t* initVal) noexcept { value = zstring(initVal); return *this; }
			generic& operator=(const zstring& initVal) noexcept { value = initVal; return *this; }
			generic& operator=(const list& initVal) noexcept { value = initVal; return *this; }


			//type conversions
			template <typename INT, typename = typename std::enable_if<std::is_integral<INT>::value,INT>::type>
			explicit inline operator INT() const { return integer(); }

			explicit inline operator float() const noexcept { return floating(); }
			explicit inline operator double() const noexcept { return floating(); }

			//internal type casting methods
			long integer() const noexcept;
			double floating() const noexcept;
			std::complex<double> complex() const noexcept;
			zstring toString(bool printArrays = false) const noexcept; //safely cast to string; slow, copies to new string
			zstring& string(); //get string ref; fast, throws exception if not actually a string.
			list& array(); //get array ref; fast, throws exception if not actually an array.

			bool numeric() const noexcept;
			bool is(std::size_t type) const noexcept;
			int type() const noexcept;


			int reducesTo(bool castStrings = false) const noexcept; //the lowest we can downcast without losing precision

			/**
			 * \brief Downcast a value as low as possible without losing precision.
			 * \param castStrings Whether to downcast strings into numeric types automatically.
			 * \return true if downcast was successful or resultant types are the same, false otherwise.
			 */
			bool reduce(bool castStrings = false) noexcept;

			/**
			 * \brief Upcast two values as much as is needed for them to be the same type.
			 * \param other The value to upcast along with this.
			 * \return true if upcast was successful or resultant types are the same, false otherwise.
			 */
			bool promote(generic* other) noexcept; //promote two numeric values to the same type


			//mathematical operators. THESE CAN THROW.
			//Note these are not fast! But should not use generic types if speed is a concern.
			generic& operator+=(const generic& other);
			generic operator+(const generic& other) const { auto tmp = *this; return tmp+=other; }
			generic& operator-=(const generic& other);
			generic operator-(const generic& other) const { auto tmp = *this; return tmp-=other; }
			generic& operator*=(const generic& other);
			generic operator*(const generic& other) const { auto tmp = *this; return tmp*=other; }
			generic& operator/=(const generic& other);
			generic operator/(const generic& other) const { auto tmp = *this; return tmp/=other; }
			generic& operator%=(const generic& other);
			generic operator%(const generic& other) const { auto tmp = *this; return tmp%=other; }
			generic operator-() const;
		};

	}
}

#endif //End if std::variant is available
