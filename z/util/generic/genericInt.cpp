#include "genericInt.h"

namespace z
{
	namespace util
	{
		genericInt::genericInt(long long init) : value(init) {}

		bool genericInt::boolean() const
		{
			return (bool)value;
		}

		long long genericInt::integer() const
		{
			return value;
		}

		double genericInt::floating() const
		{
			return value;
		}

		std::complex<double> genericInt::complex() const
		{
			return value;
		}

		zstring genericInt::string() const
		{
			return value;
		}

		bool genericInt::isInteger() const
		{
			return true;
		}

		bool genericInt::isArithmetic() const
		{
			return true;
		}

		datatype genericInt::type() const
		{
			return datatype::integer;
		}

		zstring genericInt::typeString() const
		{
			return "INT";
		}

		generic* genericInt::duplicate() const
		{
			return new genericInt(value);
		}
	}
}
