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


#include "generic/genericData.h"

#include "generic/genericDataString.h"
#include "generic/genericDataInt.h"
#include "generic/genericDataFloat.h"
#include "generic/genericDataComplex.h"
#include "generic/genericDataNull.h"
#include "generic/genericDataPointer.h"

#define g(D) z::util::generic(D)

namespace z
{
	namespace util
	{
		class generic
		{
		private:
			genericData* data;
			core::array<generic> arrayData;

			void dump();

		public:
			generic();
			generic(const generic&);
			generic(generic&&);

			generic(const core::array<generic>&);
			generic(const core::string<utf32>&);
			generic(const std::complex<double>&);
			generic(double);
			generic(void*);

			template< typename T, //numeric type
			typename = typename std::enable_if<std::is_integral<T>::value,T>::type>
			explicit generic(T input)
			{
				data = new genericDataInt(input);
			}

			~generic();


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
			bool isArray() const;
			bool isNull() const;

			void reduce();

			bool operator==(const generic&) const;
			inline bool operator!=(const generic& other) const {return !(operator==(other));}

			const generic& operator=(const generic&);
		};

		generic::generic()
		{
			data = new genericDataNull();
		}

		generic::generic(const generic& other)
		{
			if (other.data)
				data = other.data->duplicate();
			else
			{
				data = NULL;
				arrayData = other.arrayData;
			}
		}

		generic::generic(generic&& other)
		{
			data = other.data;
			arrayData = other.arrayData;
		}

		generic::generic(const core::array<generic>& input)
		{
			data = NULL;
			arrayData = input;
		}

		generic::generic(const core::string<utf32>& input)
		{
			data = new genericDataString(input);
		}

		generic::generic(const std::complex<double>& input)
		{
			data = new genericDataComplex(input);
		}

		generic::generic(double input)
		{
			data = new genericDataFloat(input);
		}

		generic::generic(void* input)
		{
			data = new genericDataPointer(input);
		}

		generic::~generic()
		{
			dump();
		}

		void generic::dump()
		{
			if (data)
				delete data;
			else
				arrayData.clear();
		}

		void generic::reduce()
		{
			if (!data) return;

			if (data->isIntegral())
			{
				long long val = data->integer();
				dump();
				data = new genericDataInt(val);
			}
			else if (data->isFloating())
			{
				double val = data->floating();
				dump();
				data = new genericDataFloat(val);
			}
			else if (data->isComplex())
			{
				std::complex<double> val = data->complex();
				dump();
				data = new genericDataComplex(val);
			}
		}

		bool generic::isArithmetic() const
		{
			if (data) return data->isArithmetic();

			return false;
		}

		bool generic::isIntegral() const
		{
			if (data) return data->isIntegral();

			return false;
		}

		bool generic::isFloating() const
		{
			if (data) return data->isFloating();

			return false;
		}

		bool generic::isComplex() const
		{
			if (data) return data->isComplex();

			return false;
		}

		bool generic::isPointer() const
		{
			if (data) return data->isPointer();

			return false;
		}

		bool generic::isString() const
		{
			if (data) return data->isString();

			return false;
		}

		bool generic::isArray() const
		{
			return !data;
		}

		bool generic::isNull() const
		{
			if (data)
				return data->isNull();
			else
				return !arrayData.length();
		}

		const bool generic::boolean() const
		{
			if (data)
				return data->boolean();
			else
				return (bool)arrayData.length();
		}

		const long long generic::integer() const
		{
			if (data)
				return data->integer();
			else
				return 0;
		}

		const double generic::floating() const
		{
			if (data)
				return data->floating();
			else
				return 0;
		}

		const std::complex<double> generic::complex() const
		{
			if (data)
				return data->complex();
			else
				return std::complex<double>(0,0);
		}

		void* generic::pointer() const
		{
			if (data)
				return data->pointer();
			else
				return NULL;
		}

		const core::string<utf32> generic::string() const
		{
			if (data)
				return data->string();
			else
			{
				core::string<utf32> result = "{";

				for (long long i=0; i<arrayData.length(); i++)
				{
					if (i) result += ",";
					result += arrayData[i].string();
				}

				result += "}";

				return result;
			}
		}

		bool generic::operator==(const generic& other) const
		{
			if (!other.data || !data)
			{
				if (!(other.data && data)) return false;

				return other.arrayData == arrayData;
			}
			else if (other.isString() || isString())
			{
				return other.string() == string();
			}
			else if (other.isArithmetic() || isArithmetic())
			{
				return other.complex() == complex();
			}
			else if (other.isPointer() && isPointer())
			{
				return other.pointer() == pointer();
			}
			else
			{
				if (other.isPointer() || isPointer()) return false;

				return true;
			}
		}

		const generic& generic::operator=(const generic& other)
		{
			dump();

			if (other.data)
				data = other.data->duplicate();
			else
			{
				data = NULL;
				arrayData = other.arrayData;
			}

			return *this;
		}

	}
}

#endif // GENERIC_H_INCLUDED
