#include "generic.h"

namespace z
{
	namespace util
	{
		generic::~generic() {}

		void* generic::pointer() const
		{
			return 0;
		}

		core::array<generic*> generic::array() const
		{
			return core::array<generic*>();
		}

		size_t generic::length() const
		{
			return 0;
		}

		int generic::find(generic* element) const
		{
			(void)element;
			return -1;
		}

		bool generic::add(generic* element)
		{
			(void)element;
			return false;
		}

		bool generic::insert(generic* element, size_t index)
		{
			(void)element;
			(void)index;
			return false;
		}

		bool generic::remove(size_t index, int count)
		{
			(void)index;
			(void)count;
			return false;
		}

		bool generic::isPointer() const
		{
			return false;
		}

		bool generic::isComplex() const
		{
			return false;
		}

		bool generic::isFloating() const
		{
			return false;
		}

		bool generic::isInteger() const
		{
			return false;
		}

		bool generic::isString() const
		{
			return false;
		}

		bool generic::isArray() const
		{
			return false;
		}

		bool generic::isArithmetic() const
		{
			return false;
		}

		bool generic::isNull() const
		{
			return false;
		}

		bool generic::operator==(const generic& other) const
		{
			if (this->type() != other.type()) return false;

			switch (this->type())
			{
				case datatype::null:
					return true;
				case datatype::pointer:
					return this->pointer() == other.pointer();
				case datatype::integer:
					return this->integer() == other.integer();
				case datatype::floating:
					return this->floating() == other.floating();
				case datatype::complex:
					return this->complex() == other.complex();
				case datatype::string:
					if (this->length() != other.length()) return false;
					return this->string() == other.string();
				case datatype::array:
					if (this->length() != other.length()) return false;
					return this->array() == other.array();
			}

			return false;
		}
	}
}
