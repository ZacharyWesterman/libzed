//This file contains specialization implementations that are identical for all string types

#ifndef ENCODING
#error "If you're seeing this, you're probably including z/core/string/shared.hpp directly, which you shouldn't do!"
#endif

namespace z
{
	namespace core
	{
		template<>
		string<ENCODING> string<ENCODING>::filter(const std::initializer_list<const std::pair<uint32_t, uint32_t>>& list, bool invert) const noexcept
		{
			string<ENCODING> result;
			for (uint32_t chr : *this)
			{
				bool matched = false;
				for (auto& range : list)
				{
					if ((chr >= range.first) && (chr <= range.second))
					{
						matched = true;
						break;
					}
				}

				if (matched != invert)
				{
					result.append(chr);
				}
			}
			return result;
		}

		template<>
		string<ENCODING> string<ENCODING>::filter(const string<ENCODING>& list, bool invert) const noexcept
		{
			string<ENCODING> result;
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

				if (matched != invert)
				{
					result.append(chr);
				}
			}
			return result;
		}

		template<>
		string<ENCODING> string<ENCODING>::filter(std::function<bool(uint32_t)> lambda) const noexcept
		{
			string<ENCODING> result;
			result.increase(length());

			for (auto chr : *this)
			{
				if (lambda(chr))
					result.append(chr);
			}

			return result;
		}

		template<>
		bool string<ENCODING>::contains(const std::initializer_list<const std::pair<uint32_t, uint32_t>>& list, bool exclusive) const noexcept
		{
			string<ENCODING> result;
			for (uint32_t chr : *this)
			{
				bool matched = false;
				for (auto& range : list)
				{
					if ((chr >= range.first) && (chr <= range.second))
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

		template<>
		bool string<ENCODING>::contains(const string<ENCODING>& list, bool exclusive) const noexcept
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

		template<>
		string<ENCODING> string<ENCODING>::cipher(const string<ENCODING>& keys, const string<ENCODING>& values) const noexcept
		{
			string<ENCODING> result;
			result.increase(length());

			const int totlen = (keys.length() <= values.length()) ? keys.length() : values.length();

			for (auto chr : *this)
			{
				bool found = false;
				for (int i=0; i<totlen; ++i)
				{
					if (chr == keys[i])
					{
						found = true;
						result.append(values[i]);
						break;
					}
				}

				if (!found)
				{
					result.append(chr);
				}
			}

			return result;
		}

		template<>
		string<ENCODING> string<ENCODING>::cipher(std::function<uint32_t(uint32_t)> lambda) const noexcept
		{
			string<ENCODING> result;
			result.increase(length());

			for (auto chr : *this)
			{
				result.append(lambda(chr));
			}

			return result;
		}

		template<>
		string<ENCODING> string<ENCODING>::ordinal(long long value) noexcept
		{
			value %= 100;
			if (((value % 10) == 3) && (value != 13))
			{
				return "rd";
			}
			else if (((value % 10) == 2) && (value != 12))
			{
				return "nd";
			}
			else if (((value % 10) == 1) && (value != 11))
			{
				return "st";
			}

			return "th";
		}

		template<>
		string<ENCODING> string<ENCODING>::words(long long value, bool ordinal) noexcept
		{
			const char* tens[] = {
				"", "",
				"twen", "thir",
				"four", "fif",
				"six", "seven",
				"eigh", "nine"
			};

			const char* ones[] = {
				"", "one",
				"two", "three",
				"four", "five",
				"six", "seven",
				"eight", "nine",
				"ten", "eleven",
				"twelve"
			};

			const char* ord[] = {
				"", "fir",
				"seco", "thi",
				"for", "fif",
				"six", "seven",
				"eigh", "nin",
				"ten", "eleven",
				"twelf"
			};

			const char* powers[] = {
				#if __x86_64__
				"quintillion",
				"quadrillion",
				"trillion",
				#endif
				"billion",
				"million",
				"thousand",
				"hundred"
			};

			const long long powvals[] = {
				#if __x86_64__
				1000000000000000000,
				1000000000000000,
				1000000000000,
				#endif
				1000000000,
				1000000,
				1000,
				100
			};

			string<ENCODING> result;
			auto orig_value = value;
			if (value < 0)
			{
				result = "negative ";
				value = -value;
			}
			else if (value == 0)
			{
				result = "zero";
			}

			bool andInt = false;
			while (value > 0)
			{
				if (value < 13)
				{
					auto one = ordinal ? ord[value] : ones[value];
					result += string<ENCODING>(andInt ? " and " : "") + one;
					value = 0;
				}
				else if (value < 20)
				{
					result += string<ENCODING>(andInt ? " and " : "") + tens[value%10] + "teen";
					value = 0;
				}
				else if (value < 100)
				{
					result += string<ENCODING>(andInt ? " and " : "") + tens[value/10];
					if (value % 10)
					{
						result += string<ENCODING>("ty-") + (ordinal ? ord[value%10] : ones[value%10]);
					}
					else
					{
						result += ordinal ? "tie" : "ty";
					}
					value = 0;
				}
				else
				{
					for (int i=0; i<7; ++i)
					{
						if (value >= powvals[i])
						{
							result += string<ENCODING>(andInt ? ", " : "") + words(value/powvals[i]) + " " + powers[i];
							value = value%powvals[i];
							andInt = true;
						}
					}
				}
			}

			if (ordinal) result += string<ENCODING>::ordinal(orig_value);
			return result;
		}
	}
}

#undef ENCODING