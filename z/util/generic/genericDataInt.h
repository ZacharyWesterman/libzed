#pragma once

namespace z
{
	namespace util
	{
		class genericDataInt : public genericData
		{
		private:
			long long data;

		public:
			genericDataInt(long long input) : data(input) {}

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

			genericDataInt* duplicate() {return new genericDataInt(data);}
		};

		const bool genericDataInt::boolean() const
		{
			return (bool)data;
		}

		const long long genericDataInt::integer() const
		{
			return data;
		}

		const double genericDataInt::floating() const
		{
			return (double)data;
		}

		const std::complex<double> genericDataInt::complex() const
		{
			return std::complex<double>(data,0);
		}

		const core::string<utf32> genericDataInt::string() const
		{
			return core::string<utf32>(data);
		}

		void* genericDataInt::pointer() const
		{
			return NULL;
		}

		bool genericDataInt::isArithmetic() const
		{
			return true;
		}

		bool genericDataInt::isIntegral() const
		{
			return true;
		}

		bool genericDataInt::isFloating() const
		{
			return false;
		}

		bool genericDataInt::isComplex() const
		{
			return false;
		}

		bool genericDataInt::isPointer() const
		{
			return false;
		}

		bool genericDataInt::isString() const
		{
			return false;
		}

		bool genericDataInt::isNull() const
		{
			return false;
		}
	}
}
