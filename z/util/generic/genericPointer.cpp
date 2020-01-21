#include "genericPointer.hpp"

namespace z
{
	namespace util
	{
		genericPointer::genericPointer(void* init) : reference(init) {}

		bool genericPointer::boolean() const
		{
			return (bool)reference;
		}

		void* genericPointer::pointer() const
		{
			return reference;
		}

		long long genericPointer::integer() const
		{
			return 0;
		}

		double genericPointer::floating() const
		{
			return 0;
		}

		std::complex<double> genericPointer::complex() const
		{
			return 0;
		}

		zstring genericPointer::string() const
		{
			return reference;
		}

		bool genericPointer::isPointer() const
		{
			return true;
		}

		bool genericPointer::isNull() const
		{
			return !reference;
		}

		datatype genericPointer::type() const
		{
			return datatype::pointer;
		}

		zstring genericPointer::typeString() const
		{
			return "POINTER";
		}

		generic* genericPointer::duplicate() const
		{
			return new genericPointer(reference);
		}
	}
}
