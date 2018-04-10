#pragma once
#ifndef GENERIC_H_INCLUDED
#define GENERIC_H_INCLUDED

#include <z/float.h>
#include <z/int.h>
#include <z/char.h>

#include <z/core/string.h>
#include <z/core/array.h>

#include <complex>
#include <z/math/complexOps.h>
#include <z/math/remainder.h>
#include <z/math/factorial.h>


namespace z
{
	namespace util
	{
		class genericData
		{
		public:
			virtual ~genericData() {}

			virtual const Int integer() const = 0;
			virtual const Float floating() const = 0;
			virtual const std::complex<Float> complex() const = 0;
			virtual const core::string<Char> string() const = 0;
			virtual void* pointer() const = 0;

			virtual bool isArithmetic() const = 0;
			virtual bool isComplex() const = 0;
			virtual bool isIntegral() const = 0;

			virtual bool isPointer() const = 0;
			virtual bool isString() const = 0;
		};

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
			bool isComplex() const;

			bool isPointer() const;
			bool isString() const;
		};

		inline const Int genericDataString::integer() const
		{
			return data.integer();
		}

		inline const Float genericDataString::floating() const
		{
			return data.value();
		}

		inline const std::complex<Float> genericDataString::complex() const
		{
			return data.complexValue();
		}

		inline const core::string<Char> genericDataString::string() const
		{
			return data;
		}

		inline void* genericDataString::pointer() const
		{
			return NULL;
		}

		inline bool genericDataString::isArithmetic() const
		{
			return false;
		}

		inline bool genericDataString::isIntegral() const
		{
			return false;
		}

		inline bool genericDataString::isComplex() const
		{
			return false;
		}

		inline bool genericDataString::isPointer() const
		{
			return false;
		}

		inline bool genericDataString::isString() const
		{
			return true;
		}
	}
}

#endif // GENERIC_H_INCLUDED
