#pragma once

#include "generic/genericNull.h"
#include "generic/genericPointer.h"
#include "generic/genericInt.h"
#include "generic/genericFloat.h"
#include "generic/genericComplex.h"
#include "generic/genericString.h"
#include "generic/genericArray.h"

#include <z/core/serializable.h>

namespace z
{
	namespace core
	{
		/**
		 * \brief Read data of a generic object from a stream.
		 *
		 * Note that this function dynamically allocates memory.
		 * Also, if the pointer is not NULL, data at that address is deallocated first.
		 *
		 * \param object Pointer to data to read from the stream.
		 * \param stream The stream to read from.
		 */
		template<>
		void serialIn(util::generic*& object, core::inputStream& stream);

		/**
		 * \brief Write data of a generic object to a stream.
		 *
		 * \param object Pointer to a single generic object to write to the stream.
		 * \param stream The stream to write to.
		 */
		template<>
		void serialOut(util::generic* const object, core::outputStream& stream);
	}
}
