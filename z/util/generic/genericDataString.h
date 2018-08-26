#pragma once

namespace z
{
	namespace util
	{
		class genericDataString : public genericData
		{
		private:
			core::string<utf32> data;

		public:
			genericDataString(const core::string<utf32>& input = core::string<utf32>()) : data(input) {}

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

			genericDataString* duplicate() {return new genericDataString(data);}
		};

		const bool genericDataString::boolean() const
		{
			auto val = data.complexValue();

			return (val.real() || val.imag());
		}

		const long long genericDataString::integer() const
		{
			return data.integer();
		}

		const double genericDataString::floating() const
		{
			return data.value();
		}

		const std::complex<double> genericDataString::complex() const
		{
			return data.complexValue();
		}

		const core::string<utf32> genericDataString::string() const
		{
			return data;
		}

		void* genericDataString::pointer() const
		{
			return NULL;
		}

		bool genericDataString::isArithmetic() const
		{
			return data.isComplex();
		}

		bool genericDataString::isFloating() const
		{
			return data.isValue();
		}

		bool genericDataString::isIntegral() const
		{
			return data.isInteger();
		}

		bool genericDataString::isComplex() const
		{
			return data.isComplex();
		}

		bool genericDataString::isPointer() const
		{
			return false;
		}

		bool genericDataString::isString() const
		{
			return true;
		}

		bool genericDataString::isNull() const
		{
			return !data.length();
		}
	}
}
