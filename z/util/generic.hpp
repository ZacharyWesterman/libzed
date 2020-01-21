#pragma once

#include "generic/genericNull.hpp"
#include "generic/genericPointer.hpp"
#include "generic/genericInt.hpp"
#include "generic/genericFloat.hpp"
#include "generic/genericComplex.hpp"
#include "generic/genericString.hpp"
#include "generic/genericArray.hpp"

#include <z/core/serializable.hpp>

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
