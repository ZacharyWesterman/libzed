#pragma once

#include "datatype.h"

#include <z/encoding.h>
#include <z/core/sizable.h>
#include <z/core/serializable.h>

#include <z/core/string.h>
#include <z/core/array.h>

#include <complex>

#ifndef ZED_GENERIC_ENCODING
#define ZED_GENERIC_ENCODING z::utf32
#endif

namespace z
{
	namespace util
	{
		class generic //: public core::sizable, public core::serializable
		{
		public:
			virtual ~generic();

			virtual bool boolean() const = 0;

			virtual void* pointer() const;
			virtual long long integer() const = 0;
			virtual double floating() const = 0;
			virtual std::complex<double> complex() const = 0;
			virtual core::string<ZED_GENERIC_ENCODING> string() const = 0;
			virtual core::array<generic*> array() const;

			virtual datatype type() const = 0;
			virtual core::string<ZED_GENERIC_ENCODING> typeString() const = 0;

			virtual bool isPointer() const;
			virtual bool isComplex() const;
			virtual bool isFloating() const;
			virtual bool isInteger() const;
			virtual bool isString() const;
			virtual bool isArray() const;

			virtual bool isArithmetic() const;
			virtual bool isNull() const;

			virtual size_t length() const;

			virtual int find(generic* element) const;
			virtual bool add(generic* element);
			virtual bool insert(generic* element, size_t index);
			virtual bool remove(size_t index, int count);

			virtual generic* duplicate() const = 0;

			virtual bool operator==(const generic& other) const;
		};
	}
}
