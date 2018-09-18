#include "genericArray.h"

namespace z
{
	namespace util
	{
		genericArray::genericArray(const core::array<generic*>& init)
		{
			for (size_t i=0; i<init.length(); i++)
				data.add(init.at(i));
		}

		genericArray::~genericArray()
		{
			for (size_t i=0; i<data.length(); i++)
				delete data.at(i);
		}

		bool genericArray::boolean() const
		{
			return false;
		}

		long long genericArray::integer() const
		{
			return 0;
		}

		double genericArray::floating() const
		{
			return 0;
		}

		std::complex<double> genericArray::complex() const
		{
			return 0;
		}

		core::string<ZED_GENERIC_ENCODING> genericArray::string() const
		{
			return "Array";
		}

		core::array<generic*> genericArray::array() const
		{
			core::array<generic*> list;
			for (size_t i=0; i<data.length(); i++)
				list.add(data.at(i));

			return list;
		}

		bool genericArray::isArray() const
		{
			return true;
		}

		bool genericArray::isNull() const
		{
			return !data.length();
		}

		datatype genericArray::type() const
		{
			return datatype::array;
		}

		core::string<ZED_GENERIC_ENCODING> genericArray::typeString() const
		{
			return "ARRAY";
		}

		size_t genericArray::length() const
		{
			return data.length();
		}

		int genericArray::find(generic* element) const
		{
			return data.find(element);
		}

		bool genericArray::add(generic* element)
		{
			if (element)
			{
				data.add(element);
				return true;
			}

			return false;
		}

		bool genericArray::insert(generic* element, size_t index)
		{
			if (element)
			{
				data.insert(element, index);
				return true;
			}

			return false;
		}

		bool genericArray::remove(size_t index, int count)
		{
			data.remove(index, count);
			return true;
		}

		generic* genericArray::duplicate() const
		{
			core::array<generic*> list;
			for (size_t i=0; i<data.length(); i++)
			{
				generic* element = data.at(i);
				if (element) list.add(element->duplicate());
			}

			return new genericArray(list);
		}
	}
}
