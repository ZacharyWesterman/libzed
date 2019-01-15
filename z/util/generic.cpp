#include "generic.h"

namespace z
{
	namespace core
	{
		void serialIn(util::generic*& object, core::inputStream& stream)
		{
			if (object)
			{
				try{delete object;} catch(...){}
			}

			int type = 0;
			serialIn(type, stream);

			if (!(stream.good() && stream.binary()))
			{
				object = new util::genericNull;
				return;
			}

			if (type == util::pointer)
			{
				void* val;
				serialIn(val, stream);
				object = new util::genericPointer(val);
			}
			else if (type == util::integer)
			{
				long long val = 0;
				serialIn(val, stream);
				object = new util::genericInt(val);
			}
			else if (type == util::floating)
			{
					double val;
					serialIn(val, stream);
					object = new util::genericFloat(val);
			}
			else if (type == util::complex)
			{
				std::complex<double> val;
				serialIn(val, stream);
				object = new util::genericComplex(val);
			}
			else if (type == util::string)
			{
				zstring str;
				serialIn(str, stream);
				object = new util::genericString(str);
			}
			else if (type == util::array)
			{
				core::refArray<util::generic*> arr;
				serialIn(arr, stream);
				object = new util::genericArray(arr);
			}
			else if (type == util::null)
			{
				object = new util::genericNull;
			}
		}

		void serialOut(const util::generic*& object, core::outputStream& stream)
		{
			if (!(stream.good() && stream.binary())) return;

			if (object)
			{
				serialOut((int)object->type(), stream);

				switch (object->type())
				{
					case util::pointer:
						serialOut(object->pointer(), stream);
						break;
					case util::integer:
						serialOut(object->integer(), stream);
						break;
					case util::floating:
						serialOut(object->floating(), stream);
						break;
					case util::complex:
						serialOut(object->complex(), stream);
						break;
					case util::string:
						serialOut(object->string(), stream);
						break;
					case util::array:
						serialOut(object->array(), stream);
						break;
					default:
						;
				}
			}
			else serialOut((int)util::null, stream);
		}

	}
}
