#include "genericString.hpp"

namespace z
{
	namespace util
	{
		genericString::genericString(const zstring& init) : data(init) {}

		bool genericString::boolean() const
		{
			std::complex<double> value = data.complex();
			return value.real() || value.imag();
		}

		long long genericString::integer() const
		{
			return data.integer();
		}

		double genericString::floating() const
		{
			return data.floating();
		}

		std::complex<double> genericString::complex() const
		{
			return data.complex();
		}

		zstring genericString::string() const
		{
			return data;
		}

		bool genericString::isInteger() const
		{
			return data.isInteger();
		}

		bool genericString::isFloating() const
		{
			return data.isFloating();
		}

		bool genericString::isComplex() const
		{
			return data.isComplex();
		}

		bool genericString::isString() const
		{
			return true;
		}

		bool genericString::isArithmetic() const
		{
			return data.isComplex();
		}

		bool genericString::isNull() const
		{
			return !data.length();
		}

		datatype genericString::type() const
		{
			return datatype::string;
		}

		zstring genericString::typeString() const
		{
			return "STRING";
		}

		int genericString::length() const
		{
			return data.length();
		}

		int genericString::find(generic* element) const
		{
			if (element)
			{
				auto substring = element->string();

				return data.find(substring);
			}

			return -1;
		}

		bool genericString::add(generic* element)
		{
			if (element)
			{
				data += element->string();
				return true;
			}

			return false;
		}

		bool genericString::insert(generic* element, int index)
		{
			if (element)
			{
				data.insert(element->string(), index);
				return true;
			}

			return false;
		}

		bool genericString::remove(int index, int count)
		{
			data.remove(index, count);
			return true;
		}

		generic* genericString::duplicate() const
		{
			return new genericString(data);
		}

		size_t genericString::size() const
		{
			return (sizeof(*this) - sizeof(zstring)) + data.size();
		}
	}
}
