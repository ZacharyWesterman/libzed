#pragma once

namespace z
{
	namespace util
	{
		class genericData
		{
		public:
			virtual ~genericData() {}

			virtual const bool boolean() const = 0;
			virtual const long long integer() const = 0;
			virtual const double floating() const = 0;
			virtual const std::complex<double> complex() const = 0;
			virtual const core::string<utf32> string() const = 0;
			virtual void* pointer() const = 0;

			virtual bool isArithmetic() const = 0;
			virtual bool isComplex() const = 0;
			virtual bool isFloating() const = 0;
			virtual bool isIntegral() const = 0;

			virtual bool isPointer() const = 0;
			virtual bool isString() const = 0;
			virtual bool isNull() const = 0;

			virtual genericData* duplicate() = 0;
		};
	}
}
