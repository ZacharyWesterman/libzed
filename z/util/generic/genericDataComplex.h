#pragma once

namespace z
{
	namespace util
	{
		class genericDataComplex : public genericData
		{
		private:
			std::complex<Float> data;

		public:
			genericDataComplex(const std::complex<Float>& input) : data(input) {}

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

			genericDataComplex* duplicate() {return new genericDataComplex(data);}
		};

		const bool genericDataComplex::boolean() const
		{
			return (data.real() || data.imag());
		}

		const Int genericDataComplex::integer() const
		{
			return (Int)data.real();
		}

		const Float genericDataComplex::floating() const
		{
			return data.real();
		}

		const std::complex<Float> genericDataComplex::complex() const
		{
			return data;
		}

		const core::string<Char> genericDataComplex::string() const
		{
			return core::string<Char>(data);
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
			bool real_int = ((Float)(Int)data.real()) == data.real();

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
