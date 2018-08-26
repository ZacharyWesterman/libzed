#pragma once

namespace z
{
	namespace util
	{
		class genericDataPointer : public genericData
		{
		private:
			void* data;

		public:
			genericDataPointer(void* input) : data(input) {}

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

			genericDataPointer* duplicate() {return new genericDataPointer(data);}
		};

		const bool genericDataPointer::boolean() const
		{
			return (bool)data;
		}

		const long long genericDataPointer::integer() const
		{
			return 0;
		}

		const double genericDataPointer::floating() const
		{
			return 0;
		}

		const std::complex<double> genericDataPointer::complex() const
		{
			return std::complex<double>(0,0);
		}

		const core::string<utf32> genericDataPointer::string() const
		{
			return core::string<utf32>(data);
		}

		void* genericDataPointer::pointer() const
		{
			return data;
		}

		bool genericDataPointer::isArithmetic() const
		{
			return false;
		}

		bool genericDataPointer::isIntegral() const
		{
			return false;
		}

		bool genericDataPointer::isFloating() const
		{
			return false;
		}

		bool genericDataPointer::isComplex() const
		{
			return false;
		}

		bool genericDataPointer::isPointer() const
		{
			return true;
		}

		bool genericDataPointer::isString() const
		{
			return false;
		}

		bool genericDataPointer::isNull() const
		{
			return !data;
		}
	}
}
