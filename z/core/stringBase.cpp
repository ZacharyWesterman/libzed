#include "stringBase.hpp"
#include "charFunctions.hpp"

namespace z
{
	namespace core
	{
		stringBase::~stringBase()
		{
			delete[] data;
		}

		uint32_t stringBase::at(int index) const noexcept
		{
			if (index < 0) index += character_ct;
			if ((index < character_ct) && (index > 0))
			{
				return getChar(index);
			}
			else
			{
				return 0;
			}
		}

		uint32_t stringBase::operator[](int index) const noexcept
		{
			return at(index);
		}

		size_t stringBase::size() const noexcept
		{
			return data_len + sizeof(*this);
		}

		int stringBase::length() const noexcept
		{
			return character_ct;
		}

		long stringBase::integer(int base, uint32_t decimal) const noexcept
		{
			if ((base < 2) || (base > 36)) return 0;

			bool negative = (getChar(0) == '-');
			long result = 0;

			int start = (negative || (getChar(0) == '+'));

			for (int i=start; i<character_ct; i++)
			{
				uint32_t chr = getChar(i);

				if (isNumeric(chr))
				{
					result *= base;
					result += numeralValue(chr);
				}
				else if (chr == decimal) break;
				else return 0;
			}

			return (negative ? -result : result);
		}

		double stringBase::floating(int base, uint32_t decimal) const noexcept
		{
			if ((base < 2) || (base > 36)) return 0;

			if (!character_ct) return 0;

			bool pastDecimal, pastExponent, negexponent;
			pastDecimal = pastExponent = negexponent = false;

			bool negative = (getChar(0) == '-');
			int start = (negative || (getChar(0) == '+'));

			if (start >= character_ct) return 0;

			double result = 0;
			double frac = 1;
			int exponent = 0;

			for (int i=start; i<character_ct; i++)
			{
				if (!isNumeric(getChar(i), base))
				{
					if (getChar(i) == decimal)
					{
						if (pastDecimal || pastExponent)
							return 0;
						else
						{
							if (i >= character_ct-1) return false;
							pastDecimal = true;
						}
					}
					else if (core::toLower(getChar(i)) == 'e')
					{
						if (pastExponent)
							return 0;
						else
						{
							pastExponent = true;
							negexponent = (getChar(i+1) == '-');
							if (negexponent || (getChar(i+1) == '+'))
								i++;
						}
					}
					else return 0;
				}
				else
				{
					if (pastExponent)
					{
						exponent *= base;
						exponent += numeralValue(getChar(i));
					}
					else if (pastDecimal)
					{
						frac /= base;
						result += (double)numeralValue(getChar(i))*frac;
					}
					else
					{
						result *= base;
						result += numeralValue(getChar(i));
					}
				}
			}

			if (pastExponent)
			{
				for (int i=0; i<exponent; i++)
				{
					if (negexponent)
						result /= base;
					else
						result *= base;
				}
			}

			return (negative ? -result : result);
		}

		std::complex<double> stringBase::complex(int base, uint32_t decimal) const noexcept
		{
			if ((base < 2) || (base > 36)) return 0;
			if (!character_ct) return 0;

			int start = ((getChar(0) == '-') || (getChar(0) == '+'));
			bool imag = false;
			bool imagEnd = core::toLower(getChar(character_ct-1)) == 'i';
			for (int i=start; i<character_ct; ++i)
			{
				if (core::toLower(getChar(i)) == 'i') imag = true;
				if (((getChar(i) == '-') || (getChar(i) == '+')) && (base < 14) && i && (core::toLower(getChar(i-1)) != 'e'))
				{
					if (!(imag ^ imagEnd)) return 0;
					auto sub1 = substr(0,i-imag);
					auto sub2 = substr(i,character_ct-i-imagEnd);

					if (imag)
					{
						return std::complex<double>(sub2.floating(base, decimal),sub1.floating(base, decimal));
					}
					else
					{
						return std::complex<double>(sub1.floating(base, decimal),sub2.floating(base, decimal));
					}
				}
			}

			if (imagEnd)
				return std::complex<double>(0,substr(0,character_ct-1).floating(base,decimal));
			else
				return floating(base,decimal);
		}

		int stringBase::count(const stringBase& other) const noexcept
		{
			if (!other.character_ct) return 0;

			int occurrence = 0;

			int other_i = 0;
			for (int i=0; i<character_ct; i++)
			{
				if (data[i] == other.data[other_i])
				{
					other_i++;
					if (other_i >= other.character_ct)
						occurrence++;

					if (!occurrence) return (i - other_i + 1);
				}
				else
				{
					if ((character_ct - i) <= other.character_ct)
						return occurrence;

					other_i = 0;
				}
			}

			return occurrence;
		}

		int stringBase::find(const stringBase& other, int occurrence) const noexcept
		{
			return findAfter(other,0,occurrence);
		}

		bool stringBase::contains(const stringBase& list, bool exclusive) const noexcept
		{
			for (uint32_t chr : *this)
			{
				bool matched = false;
				for (uint32_t other : list)
				{
					if (chr == other)
					{
						matched = true;
						break;
					}
				}

				//if (matched && !exclusive) or (!matched && exclusive)
				//then we know the result, exit right away.
				if (matched ^ exclusive)
				{
					return matched;
				}
			}

			//will ony get here if (!matched && !exclusive) or (matched && exclusive)
			//therefore, matched := exclusive
			return exclusive;
		}

		stringIterator stringBase::begin() const noexcept
		{
			return stringIterator(data, 0, format());
		}

		stringIterator stringBase::end() const noexcept
		{
			return stringIterator(data, character_ct, format());
		}
	}

}
