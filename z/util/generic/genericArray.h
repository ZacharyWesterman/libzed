#pragma once

#include "generic.h"
#include <z/core/refArray.h>

namespace z
{
	namespace util
	{
		/**
		 * \brief An implementation of the generic type for arrays.
		 */
		class genericArray : public generic
		{
		private:
			core::refArray<generic*> data;

		public:
			/**
			 * \brief Array constructor.
			 *
			 * Note that the initializing data is assumed to be allocated exclusively for this object.
			 * As such, the objects are not duplicated, the pointers are just copied.
			 * Control is passed to this object, so when it is deleted or goes out of scope, all contained items are deleted.
			 *
			 * \param init An array of generics to initialize this object as.
			 */
			genericArray(const core::array<generic*>& init);

			/**
			 * \brief Empty constructor.
			 *
			 * Creates this object as an empty array.
			 */
			genericArray();

			~genericArray();

			bool boolean() const;

			long long integer() const;
			double floating() const;
			std::complex<double> complex() const;
			core::string<> string() const;
			core::array<generic*> array() const;

			bool isArray() const;

			bool isNull() const;

			size_t length() const;

			int find(generic* element) const;
			bool add(generic* element);
			bool insert(generic* element, size_t index);
			bool remove(size_t index, int count);

			datatype type() const;
			core::string<> typeString() const;

			generic* duplicate() const;
		};
	}
}
