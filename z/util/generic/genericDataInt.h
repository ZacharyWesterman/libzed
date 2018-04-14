#pragma once

namespace z
{
	namespace util
	{
		class genericDataInt : public genericData
		{
		private:
			Int data;

		public:
			genericDataInt(Int input) : data(input) {}

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

			genericDataInt* duplicate() {return new genericDataInt(data);}
		};

		const Int genericDataInt::integer() const
		{
			return data;
		}

		const Float genericDataInt::floating() const
		{
			return (Float)data;
		}

		const std::complex<Float> genericDataInt::complex() const
		{
			return std::complex<Float>(data,0);
		}

		const core::string<Char> genericDataInt::string() const
		{
			return core::string<Char>(data);
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
