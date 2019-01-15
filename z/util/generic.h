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
		void serialIn(util::generic*& object, core::inputStream& stream);

		void serialOut(const util::generic*& object, core::inputStream& stream);
	}
}
