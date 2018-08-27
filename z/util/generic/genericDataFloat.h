#pragma once

namespace z
{
	namespace util
	{
		class genericDataFloat : public genericData
		{
		private:
			double data;

		public:
			genericDataFloat(double input) : data(input) {}

			const bool boolean() const;
			const long long integer() const;
			const double floating() const;
			const std::complex<double> complex() const;
			const core::string<utf32> string() const;
			void* pointer() const;

			bool isArithmetic() const;
			bool isIntegral() const;
			bool isFloating() const;
			bool isComplex() const;

			bool isPointer() const;
			bool isString() const;
			bool isNull() const;

			genericDataFloat* duplicate() {return new genericDataFloat(data);}
		};

		const bool genericDataFloat::boolean() const
		{
			return (bool)data;
		}

		const long long genericDataFloat::integer() const
		{
			return (long long)data;
		}

		const double genericDataFloat::floating() const
		{
			return data;
		}

		const std::complex<double> genericDataFloat::complex() const
		{
			return std::complex<double>(data,0);
		}

		const core::string<utf32> genericDataFloat::string() const
		{
			return core::string<utf32>(data);
		}

		void* genericDataFloat::pointer() const
		{
			return NULL;
		}

		bool genericDataFloat::isArithmetic() const
		{
			return true;
		}

		bool genericDataFloat::isIntegral() const
		{
			return (double)(long long)data == data;
		}

		bool genericDataFloat::isFloating() const
		{
			return true;
		}

		bool genericDataFloat::isComplex() const
		{
			return false;
		}

		bool genericDataFloat::isPointer() const
		{
			return false;
		}

		bool genericDataFloat::isString() const
		{
			return false;
		}

		bool genericDataFloat::isNull() const
		{
			return false;
		}
	}
}
