#pragma once

namespace z
{
	namespace util
	{
		class genericDataNull : public genericData
		{
		public:
			genericDataNull() {}

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

			genericDataNull* duplicate() {return new genericDataNull();}
		};

		const bool genericDataNull::boolean() const
		{
			return false;
		}

		const long long genericDataNull::integer() const
		{
			return 0;
		}

		const double genericDataNull::floating() const
		{
			return 0;
		}

		const std::complex<double> genericDataNull::complex() const
		{
			return std::complex<double>(0,0);
		}

		const core::string<utf32> genericDataNull::string() const
		{
			return core::string<utf32>();
		}

		void* genericDataNull::pointer() const
		{
			return NULL;
		}

		bool genericDataNull::isArithmetic() const
		{
			return false;
		}

		bool genericDataNull::isIntegral() const
		{
			return false;
		}

		bool genericDataNull::isFloating() const
		{
			return false;
		}

		bool genericDataNull::isComplex() const
		{
			return false;
		}

		bool genericDataNull::isPointer() const
		{
			return false;
		}

		bool genericDataNull::isString() const
		{
			return false;
		}

		bool genericDataNull::isNull() const
		{
			return true;
		}
	}
}
