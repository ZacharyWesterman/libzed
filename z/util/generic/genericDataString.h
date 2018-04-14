#pragma once

namespace z
{
	namespace util
	{
		class genericDataString : public genericData
		{
		private:
			core::string<Char> data;

		public:
			genericDataString(const core::string<Char>& input = core::string<Char>()) : data(input) {}

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

			genericDataString* duplicate() {return new genericDataString(data);}
		};

		const Int genericDataString::integer() const
		{
			return data.integer();
		}

		const Float genericDataString::floating() const
		{
			return data.value();
		}

		const std::complex<Float> genericDataString::complex() const
		{
			return data.complexValue();
		}

		const core::string<Char> genericDataString::string() const
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
