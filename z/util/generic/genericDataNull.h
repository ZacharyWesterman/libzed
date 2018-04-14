#pragma once

namespace z
{
	namespace util
	{
		class genericDataNull : public genericData
		{
		public:
			genericDataNull() {}

			const Int integer() const;
			const Float floating() const;
			const std::complex<Float> complex() const;
			const core::string<Char> string() const;
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

		const Int genericDataNull::integer() const
		{
			return 0;
		}

		const Float genericDataNull::floating() const
		{
			return 0;
		}

		const std::complex<Float> genericDataNull::complex() const
		{
			return std::complex<Float>(0,0);
		}

		const core::string<Char> genericDataNull::string() const
		{
			return core::string<Char>();
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
