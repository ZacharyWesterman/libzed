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

			genericDataPointer* duplicate() {return new genericDataPointer(data);}
		};

		const Int genericDataPointer::integer() const
		{
			return 0;
		}

		const Float genericDataPointer::floating() const
		{
			return 0;
		}

		const std::complex<Float> genericDataPointer::complex() const
		{
			return std::complex<Float>(0,0);
		}

		const core::string<Char> genericDataPointer::string() const
		{
			return core::string<Char>(data);
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
