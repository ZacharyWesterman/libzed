#pragma once

namespace z
{
	namespace util
	{
		class genericDataFloat : public genericData
		{
		private:
			Float data;

		public:
			genericDataFloat(Float input) : data(input) {}

			const bool boolean() const;
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

			genericDataFloat* duplicate() {return new genericDataFloat(data);}
		};

		const bool genericDataFloat::boolean() const
		{
			return (bool)data;
		}

		const Int genericDataFloat::integer() const
		{
			return (Int)data;
		}

		const Float genericDataFloat::floating() const
		{
			return data;
		}

		const std::complex<Float> genericDataFloat::complex() const
		{
			return std::complex<Float>(data,0);
		}

		const core::string<Char> genericDataFloat::string() const
		{
			return core::string<Char>(data);
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
			return (Float)(Int)data == data;
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
