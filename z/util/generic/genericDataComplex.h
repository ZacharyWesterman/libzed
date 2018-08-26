#pragma once

namespace z
{
	namespace util
	{
		class genericDataComplex : public genericData
		{
		private:
			std::complex<double> data;

		public:
			genericDataComplex(const std::complex<double>& input) : data(input) {}

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

			genericDataComplex* duplicate() {return new genericDataComplex(data);}
		};

		const bool genericDataComplex::boolean() const
		{
			return (data.real() || data.imag());
		}

		const long long genericDataComplex::integer() const
		{
			return (long long)data.real();
		}

		const double genericDataComplex::floating() const
		{
			return data.real();
		}

		const std::complex<double> genericDataComplex::complex() const
		{
			return data;
		}

		const core::string<utf32> genericDataComplex::string() const
		{
			return core::string<utf32>(data);
		}

		void* genericDataComplex::pointer() const
		{
			return NULL;
		}

		bool genericDataComplex::isArithmetic() const
		{
			return true;
		}

		bool genericDataComplex::isIntegral() const
		{
			bool real_int = ((double)(long long)data.real()) == data.real();

			return real_int && !data.imag();
		}

		bool genericDataComplex::isFloating() const
		{
			return !data.imag();
		}

		bool genericDataComplex::isComplex() const
		{
			return (bool)data.imag();
		}

		bool genericDataComplex::isPointer() const
		{
			return false;
		}

		bool genericDataComplex::isString() const
		{
			return false;
		}

		bool genericDataComplex::isNull() const
		{
			return false;
		}
	}
}
