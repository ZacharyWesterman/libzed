#include "genericNull.h"

namespace z
{
	namespace util
	{
		bool genericNull::boolean() const
		{
			return false;
		}

		long long genericNull::integer() const
		{
			return 0;
		}

		double genericNull::floating() const
		{
			return 0;
		}

		std::complex<double> genericNull::complex() const
		{
			return 0;
		}

		zstring genericNull::string() const
		{
			return zstring();
		}

		datatype genericNull::type() const
		{
			return datatype::null;
		}

		zstring genericNull::typeString() const
		{
			return "NULL";
		}

		bool genericNull::isNull() const
		{
			return true;
		}

		generic* genericNull::duplicate() const
		{
			return new genericNull;
		}
	}
}
