#include "genericFloat.h"

namespace z
{
	namespace util
	{
		genericFloat::genericFloat(double init) : value(init) {}

		bool genericFloat::boolean() const
		{
			return (bool)value;
		}

		long long genericFloat::integer() const
		{
			return value;
		}

		double genericFloat::floating() const
		{
			return value;
		}

		std::complex<double> genericFloat::complex() const
		{
			return value;
		}

		core::string<ZED_GENERIC_ENCODING> genericFloat::string() const
		{
			return value;
		}

		bool genericFloat::isInteger() const
		{
			return ((double)((long long)value)) == value;
		}

		bool genericFloat::isFloating() const
		{
			return true;
		}

		bool genericFloat::isArithmetic() const
		{
			return true;
		}

		datatype genericFloat::type() const
		{
			return datatype::floating;
		}

		core::string<ZED_GENERIC_ENCODING> genericFloat::typeString() const
		{
			return "FLOAT";
		}

		generic* genericFloat::duplicate() const
		{
			return new genericFloat(value);
		}
	}
}
