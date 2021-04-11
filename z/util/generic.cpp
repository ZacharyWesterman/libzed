#include "generic.hpp"

//Only compile if std::variant is available
#if __cplusplus >= 201703L

#include "genericExceptions.hpp"

#include "../core/join.hpp"

namespace z
{
	namespace util
	{
		long generic::integer() const noexcept
		{
			switch (value.index())
			{
				case generic::INT:
					return std::get<generic::INT>(value);
				case generic::FLOAT:
					return std::get<generic::FLOAT>(value);
				case generic::COMPLEX:
					return std::get<generic::COMPLEX>(value).real();
				case generic::STRING:
					return std::get<generic::STRING>(value).integer();
				default:
					return 0; //arrays can't really be cast to a number...
			}
		}

		double generic::floating() const noexcept
		{
			switch (value.index())
			{
				case generic::INT:
					return std::get<generic::INT>(value);
				case generic::FLOAT:
					return std::get<generic::FLOAT>(value);
				case generic::COMPLEX:
					return std::get<generic::COMPLEX>(value).real();
				case generic::STRING:
					return std::get<generic::STRING>(value).floating();
				default:
					return 0; //arrays can't really be cast to a number...
			}
		}

		std::complex<double> generic::complex() const noexcept
		{
			switch (value.index())
			{
				case generic::INT:
					return std::get<generic::INT>(value);
				case generic::FLOAT:
					return std::get<generic::FLOAT>(value);
				case generic::COMPLEX:
					return std::get<generic::COMPLEX>(value);
				case generic::STRING:
					return std::get<generic::STRING>(value).complex();
				default:
					return 0; //arrays can't really be cast to a number...
			}
		}

		zstring generic::toString(bool printArrays) const noexcept
		{
			const int ix = value.index();
			if (ix == generic::INT)
				return std::get<generic::INT>(value);
			else if (ix == generic::FLOAT)
				return std::get<generic::FLOAT>(value);
			else if (ix == generic::COMPLEX)
				return std::get<generic::COMPLEX>(value);
			else if (ix == generic::STRING)
				return std::get<generic::STRING>(value);
			else if (printArrays && (ix == generic::ARRAY))
			{
				zstring res = '{';
				const z::core::array<generic>& arr = std::get<generic::ARRAY>(value);
				for (int i=0; i<arr.length(); ++i)
				{
					if (i) res.append(',');
					res.append(arr[i].toString(true));
				}
				res.append('}');
				return res;
			}

			return ""; //Not able to really convert to a string so just give null.
		}

		zstring& generic::string()
		{
			return std::get<generic::STRING>(value);
		}

		const zstring& generic::string() const
		{
			return std::get<generic::STRING>(value);
		}

		generic::list& generic::array()
		{
			return std::get<generic::ARRAY>(value);
		}

		const generic::list& generic::array() const
		{
			return std::get<generic::ARRAY>(value);
		}

		bool generic::numeric() const noexcept
		{
			return (value.index() < generic::STRING) && (value.index() > generic::VOID);
		}

		bool generic::is(std::size_t type) const noexcept
		{
			return type == value.index();
		}

		int generic::type() const noexcept
		{
			return value.index();
		}

		const char* generic::typeString() const noexcept
		{
			const char* typeOpts[6] = {
				"void",
				"int",
				"float",
				"complex",
				"string",
				"array"
			};

			//shouldn't ever be out of range, just a sanity check.
			const unsigned int ix = value.index();
			return (ix < 6) ? typeOpts[ix] : "ERROR";
		}

		int generic::reducesTo(bool castStrings) const noexcept
		{
			const int ix = value.index();

			//arrays cannot be downcast.
			if (ix == generic::ARRAY)
			{
				return false;
			}
			//if this is a string, downcast to the lowest numeric value
			else if (ix == generic::STRING)
			{
				if (!castStrings) return false;

				const zstring& str = std::get<generic::STRING>(value);
				switch (str.type())
				{
					case core::zstr::integer:
						return generic::INT;
					case core::zstr::floating:
						return generic::FLOAT;
					case core::zstr::complex:
						return generic::COMPLEX;
					default:
						return generic::STRING;
				}
			}
			else if (ix == generic::COMPLEX)
			{
				const std::complex<double> cval = std::get<generic::COMPLEX>(value);
				if (cval.imag()) return generic::COMPLEX;

				const double fval = cval.real();
				if (double(long(fval)) != fval) return generic::FLOAT;
			}
			else if (ix == generic::FLOAT)
			{
				const double fval = std::get<generic::FLOAT>(value);
				if (double(long(fval)) != fval) return generic::FLOAT;
			}

			return generic::INT;
		}

		bool generic::reduce(bool castStrings) noexcept
		{
			const int ix = value.index();

			//arrays cannot be downcast.
			if (ix == generic::ARRAY)
			{
				return false;
			}
			//if this is a string, downcast to the lowest numeric value
			else if (ix == generic::STRING)
			{
				if (!castStrings) return false;

				zstring& str = std::get<generic::STRING>(value);
				switch (str.type())
				{
					case core::zstr::integer:
						value = str.integer();
						break;
					case core::zstr::floating:
						value = str.floating();
						break;
					case core::zstr::complex:
						value = str.complex();
						break;
					default:
						return false;
				}
			}
			else if (ix == generic::COMPLEX)
			{
				const std::complex<double> cval = std::get<generic::COMPLEX>(value);
				if (!cval.imag())
				{
					const double fval = cval.real();
					if (double(long(fval)) == fval)
						value = long(fval);
					else
						value = fval;
				}
			}
			else if (ix == generic::FLOAT)
			{
				const double fval = std::get<generic::FLOAT>(value);
				if (double(long(fval)) == fval)
					value = long(fval);
			}

			return true;
		}

		bool generic::promote(generic* other) noexcept
		{
			if (!other) return false;

			const int ix1 = value.index();
			const int ix2 = other->value.index();
			if (ix1 == ix2) return true;

			if (ix1 > ix2)
			{
				if (ix1 == generic::STRING)
					other->value = other->toString();
				else if (ix1 == generic::COMPLEX)
					other->value = other->complex();
				else if (ix1 == generic::FLOAT)
					other->value = other->floating();
			}
			else
			{
				if (ix2 == generic::STRING)
					value = toString();
				else if (ix2 == generic::COMPLEX)
					value = complex();
				else if (ix2 == generic::FLOAT)
					value = floating();
			}

			return true;
		}

		generic& generic::operator+=(const generic& other)
		{
			if (!(numeric() && other.numeric())) throw nonNumber();

			generic alt = other;
			promote(&alt);

			const int ix = value.index();
			if (ix == generic::COMPLEX)
				value = std::get<generic::COMPLEX>(value) + std::get<generic::COMPLEX>(alt.value);
			else if (ix == generic::FLOAT)
				value = std::get<generic::FLOAT>(value) + std::get<generic::FLOAT>(alt.value);
			else
				value = std::get<generic::INT>(value) + std::get<generic::INT>(alt.value);

			return *this;
		}

		generic& generic::operator-=(const generic& other)
		{
			if (!(numeric() && other.numeric())) throw nonNumber();

			generic alt = other;
			promote(&alt);

			const int ix = value.index();
			if (ix == generic::COMPLEX)
				value = std::get<generic::COMPLEX>(value) - std::get<generic::COMPLEX>(alt.value);
			else if (ix == generic::FLOAT)
				value = std::get<generic::FLOAT>(value) - std::get<generic::FLOAT>(alt.value);
			else
				value = std::get<generic::INT>(value) - std::get<generic::INT>(alt.value);

			return *this;
		}

		generic& generic::operator*=(const generic& other)
		{
			if (!(numeric() && other.numeric())) throw nonNumber();

			generic alt = other;
			promote(&alt);

			const int ix = value.index();
			if (ix == generic::COMPLEX)
				value = std::get<generic::COMPLEX>(value) * std::get<generic::COMPLEX>(alt.value);
			else if (ix == generic::FLOAT)
				value = std::get<generic::FLOAT>(value) * std::get<generic::FLOAT>(alt.value);
			else
				value = std::get<generic::INT>(value) * std::get<generic::INT>(alt.value);

			return *this;
		}

		generic& generic::operator/=(const generic& other)
		{
			if (!(numeric() && other.numeric())) throw nonNumber();

			generic alt = other;
			promote(&alt);

			const int ix = value.index();
			if (ix == generic::COMPLEX)
				value = std::get<generic::COMPLEX>(value) / std::get<generic::COMPLEX>(alt.value);
			else if (ix == generic::FLOAT)
				value = std::get<generic::FLOAT>(value) / std::get<generic::FLOAT>(alt.value);
			else
				value = std::get<generic::INT>(value) / std::get<generic::INT>(alt.value);

			return *this;
		}

		generic& generic::operator%=(const generic& other)
		{
			if (!(numeric() && other.numeric())) throw nonNumber();

			value = integer() % other.integer();

			return *this;
		}

		generic generic::operator-() const
		{
			if (!numeric()) throw nonNumber();

			const int ix = value.index();
			if (ix == generic::COMPLEX)
				return -std::get<generic::COMPLEX>(value);
			else if (ix == generic::FLOAT)
				return -std::get<generic::FLOAT>(value);
			else
				return -std::get<generic::INT>(value);

			return *this;
		}

		bool generic::operator==(const generic& other) const
		{
			if (this == &other) return true;

			const int ix = value.index();
			const int oix = other.value.index();
			if (ix != oix) return false;

			if (ix == generic::ARRAY)
				return std::get<generic::ARRAY>(value) == std::get<generic::ARRAY>(other.value);
			else if (ix == generic::STRING)
				return std::get<generic::STRING>(value) == std::get<generic::STRING>(other.value);
			else if (ix == generic::COMPLEX)
				return std::get<generic::COMPLEX>(value) == std::get<generic::COMPLEX>(other.value);
			else if (ix == generic::FLOAT)
				return std::get<generic::FLOAT>(value) == std::get<generic::FLOAT>(other.value);
			else if (ix == generic::INT)
				return std::get<generic::INT>(value) == std::get<generic::INT>(other.value);
			else
				return true; //Null types are always equivalent.
		}

		bool generic::equivalent(const generic& other) const
		{
			if (this == &other) return true;

			const int ix = value.index();
			const int oix = other.value.index();

			//don't duplicate arrays (may be large)
			if ((ix == generic::ARRAY) || (oix == generic::ARRAY))
			{
				if (ix != oix) return false;
				return std::get<generic::ARRAY>(value) == std::get<generic::ARRAY>(other.value);
			}

			//don't duplicate strings (may be large)
			if ((ix == generic::STRING) && (oix == generic::STRING))
			{
				return std::get<generic::STRING>(value) == std::get<generic::STRING>(other.value);
			}
			else if (ix == generic::STRING)
			{
				return std::get<generic::STRING>(value) == other.toString();
			}
			else if (oix == generic::STRING)
			{
				return toString() == std::get<generic::STRING>(other.value);
			}

			generic a(*this), b(other);
			a.promote(&b);
			return a.operator==(b);
		}

	}
}

#endif //End if std::variant is available
