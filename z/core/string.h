#pragma once
#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#ifndef null
    #define null 0
#endif // null
#ifndef NULL
    #define NULL 0
#endif // NULL

//this is for initializing numbers properly
#include <type_traits>
#include <complex>

#include <z/float.h>
#include <z/int.h>

#include "charFunctions.h"

#define num_bufsiz 16
#define num_precision 6

//magic number for determining when to round numbers,
//e.g. at the ~0.5 mark.
#define num_round_magic 0.499999958629814528210388857587539

#define cs(s) z::core::string<char>(s)
#define ws(s) z::core::string<wchar_t>(s)

namespace z
{
    namespace core
    {

        //"buffer" is assumed to be 2*num_bufsiz + 7 characters long
        //returns number of characters in resultant string
        template<typename CHAR>
        static Int num_to_cstring(Float number, CHAR* buffer, Int base)
        {
            Float_cast dbl_cst = {.value = number};

            bool use_scientific = (dbl_cst.expUnBias() > 50) ||
                                  (dbl_cst.expUnBias() < (-50));

            Int buffer_pos = 0;

            Float fpart;
            long ipart;
            long exponent = 0;

            if (use_scientific)
            {
                while (dbl_cst.value >= 10)
                {
                    dbl_cst.value /= 10;
                    exponent++;
                }
            }

            ipart = dbl_cst.value;
            fpart = dbl_cst.value - ipart;

                //std::cout << fpart << ":" << ipart << std::endl;

                CHAR inv_ibuf[num_bufsiz];
                CHAR inv_fbuf[num_bufsiz];
                CHAR inv_xbuf[4];

                Int ibufsiz = 0;
                Int fbufsiz = 0;
                Int xbufsiz = 0;


                if (ipart < 0)
                {
                    buffer[buffer_pos] = (CHAR)45;
                    buffer_pos++;

                    ipart = -ipart;
                }
                else if (fpart < 0)
                {
                    buffer[buffer_pos] = (CHAR)45;
                    buffer_pos++;

                    fpart = -fpart;
                }


                if (ipart == 0)
                {
                    inv_ibuf[0] = (CHAR)48;
                    ibufsiz++;
                }


                while (ipart != 0)
                {
                    inv_ibuf[ibufsiz] = (CHAR)((ipart % 10) + 48);
                    ipart /= 10;

                    ibufsiz++;
                }


                while ((fpart != 0) && (fbufsiz < num_precision))
                {
                    fpart *= 10;

                    Int frac_char = fpart;

                    inv_fbuf[fbufsiz] = (CHAR)(frac_char + 48);

                    fbufsiz++;
                    fpart -= frac_char;
                }

                while (exponent != 0)
                {
                    inv_xbuf[xbufsiz] = (CHAR)((exponent % 10) + 48);
                    exponent /= 10;

                    xbufsiz++;
                }



                if (fpart >= num_round_magic)
                {
                    Int i = fbufsiz - 1;

                    inv_fbuf[i]++;

                    while ((i > 0) && (inv_fbuf[i] > (CHAR)57))
                    {
                        fbufsiz--;
                        inv_fbuf[i-1]++;

                        i--;
                    }

                    if (inv_fbuf[0] > (CHAR)57)
                    {
                        fbufsiz--;
                        inv_ibuf[0]++;

                        i=0;

                        while ((i < ibufsiz) && (inv_ibuf[i] > (CHAR)57))
                        {
                            inv_ibuf[i] = (CHAR)48;

                            if (i+1 == ibufsiz)
                            {
                                if (ibufsiz < num_bufsiz)
                                {
                                    inv_ibuf[ibufsiz] = (CHAR)49;

                                    ibufsiz++;
                                }
                            }
                            else
                            {
                                inv_ibuf[i+1]++;
                            }

                            i++;
                        }
                    }
                }

                for (Int i=ibufsiz-1; i>=0; i--)
                {
                    buffer[buffer_pos] = inv_ibuf[i];
                    buffer_pos++;
                }

                if (fbufsiz)
                {
                    buffer[buffer_pos] = (CHAR)46; // '.'
                    buffer_pos++;

                    Int frac_len = 0;

                    for (Int f=0; f<fbufsiz; f++)
                    {
                        buffer[buffer_pos + f] = inv_fbuf[f];

                        if (inv_fbuf[f] > 48)
                            frac_len = f + 1;
                    }

                    buffer_pos += frac_len;
                }


                if (xbufsiz)
                {
                    buffer[buffer_pos] = (CHAR)69; //'E'
                    buffer_pos++;

                    for (Int i=xbufsiz-1; i>=0; i--)
                    {
                        buffer[buffer_pos] = inv_xbuf[i];
                        buffer_pos++;
                    }
                }

                //buffer is null-terminated
                buffer[buffer_pos] = (CHAR)0;

                if (buffer[buffer_pos-1] == (CHAR)46) //remove terminal decimals
                {
                    //don't allow -0
                    if ((buffer[0] == (CHAR)45) &&
                        (buffer[1] == (CHAR)48))
                    {

                        buffer[0] = (CHAR)48; //'0'
                        buffer[1] = (CHAR)0;

                        buffer_pos -= 2;
                    }
                    else
                    {
                        buffer[buffer_pos-1] = (CHAR)0;

                        buffer_pos--;
                    }
                }



                return buffer_pos;
        }

		//"buffer" is assumed to be 2*num_bufsiz + 7 characters long
		//returns number of characters in resultant string
		template<typename CHAR>
		static Int int_to_cstring(Int number, CHAR* buffer, Int base)
		{
			Int buffer_pos, start;


			if (number < 0)
			{
				buffer[0] = '-';
				start = 1;
			}
			else if (number > 0)
			{
				start = 0;
			}
			else
			{
				buffer[0] = '0';
				buffer[1] = 0;

				return 2;
			}

			buffer_pos = start;

			while (number > 0)
			{
				buffer[buffer_pos] = numeral(number % base);
				number /= base;

				buffer_pos++;
			}

			for (Int i=0; i<((buffer_pos-start)/2); i++)
			{
				Int from = i + start;
				Int to = buffer_pos - 1 - i;

				CHAR tmp = buffer[from];
				buffer[from] = buffer[to];
				buffer[to] = tmp;
			}

			return buffer_pos;
		}

		//"buffer" is assumed to be 2*num_bufsiz + 7 characters long
		//returns number of characters in resultant string
		template<typename CHAR>
		static Int ptr_to_cstring(void* ptr, CHAR* buffer)
		{
			Int buffer_pos;
			long long number = (long long)ptr;

			const Int buf_chars = sizeof(void*)-1+2; //0x01234567


			buffer_pos = buf_chars;

			while (buffer_pos >= 2)
			{
				buffer[buffer_pos] = numeral(number % 16);
				number = number >> 4;

				buffer_pos--;
			}

			// for (Int i=0; i<((buffer_pos-start)/2); i++)
			// {
			// 	Int from = i + start;
			// 	Int to = buffer_pos - 1 - i;
			//
			// 	CHAR tmp = buffer[from];
			// 	buffer[from] = buffer[to];
			// 	buffer[to] = tmp;
			// }
			buffer[0] = '0';
			buffer[1] = 'x';
			buffer[buf_chars+1] = 0;

			return buf_chars+2;
		}

        /**
         * \brief A template class for character strings.
         *
         * Strings are expected to be of either \b char or \b wchar_t
         * types, but also works with other integer types. <BR>
         * Note that all integer types from \b wchar_t and larger
         * are assumed to have \b UTF-16 encoding. <BR><BR>
         * Additionally, this class is type-independent, so
         * it handles all type conversions automatically.
         *
         */
        template <typename CHAR>
        class string
        {
            static_assert(std::is_integral<CHAR>::value,
                          "Invalid character type");

        private:
			Int max_length;
            Int current_length;
            CHAR* string_array;

            void append_string(const CHAR* buffer, Int bufsiz);
            void append_char(CHAR singleChar);

            bool found_sub_string_at(Int pos,
                                     const CHAR* substr,
                                     Int substr_len) const;

            void replace_sub_string_at_with(Int beg_index,
                                            Int end_index,
                                            const CHAR* sub_str,
                                            Int substr_len);

            Int lessthan_equal_greater(const CHAR* other, Int str_size) const;

            void remove_portion(Int beg_index, Int end_index);


        public:
            string();
            ~string();

            string(string&& other);


			string(const char* buffer);
            string(const wchar_t* buffer);

            string(const string& other);

            template <typename CHAR_2>
            string(const string<CHAR_2>& other);

            explicit string(const char& character);
            explicit string(const wchar_t& character);

            template<
                typename T, //numeric type
                typename = typename std::enable_if
                        <std::is_arithmetic<T>::value,T>::type>
            string(const T& number, Int base = 10);

			template<typename T>
            string(T* pointer);

            template<
                typename T, //numeric type
                typename = typename std::enable_if
                        <std::is_arithmetic<T>::value,T>::type>
            explicit string(const std::complex<T>& number, Int base = 10);



            Int size() const;
            inline Int length() const;
            inline const CHAR* str() const;



            inline bool operator==(const string& other) const;
            inline bool operator!=(const string& other) const;
            inline bool operator>(const string& other) const;
            inline bool operator>=(const string& other) const;
            inline bool operator<(const string& other) const;
            inline bool operator<=(const string& other) const;
            inline const string& operator+=(const string& other);
            inline const string operator+(const string& other) const;



            CHAR at(Int index) const;
            CHAR& at(Int index);
            inline CHAR operator[](Int index) const;
            inline CHAR& operator[](Int index);

            Int find(const string& sub_string) const;
            Int find(const string& sub_string, Int n) const;
            Int findAfter(const string& sub_string, Int n) const;
            Int findLast(const string& sub_string) const;


            inline void replace(Int start_pos,
                                Int end_pos,
                                const string& new_string);

            bool replace(const string& sub_string,
                         const string& new_string);

            bool replace(const string& sub_string,
                         Int n,
                         const string& new_string);


            const string substr(Int start_index, Int end_index) const;

            Int count(const string& sub_string) const;

            inline void remove(Int start_index, Int end_index);

			/**
			 * \brief Remove one character from the string.
			 *
			 * Removes the character from the string at the given index.
			 *
			 * \param index the index of the character to remove.
			 */
			inline void remove(Int index) {this->remove(index, index);}

            bool foundAt(const string& sub_string, Int position) const;
            bool foundEndAt(const string& sub_string, Int position) const;

            inline bool beginsWith(const string& sub_string) const;
            bool beginsWith(const string& sub_string, CHAR pad_char) const;

            inline bool endsWith(const string& sub_string) const;


            void clear();

            const string& operator=(const string&);

            template <typename CHAR_2>
            const string<CHAR>& operator=(const string<CHAR_2>& other);


            inline const string<char> narrow() const;
            inline const string<wchar_t> wide() const;

            const string<CHAR> upper() const;
            const string<CHAR> lower() const;

            Float value(Int base = 10) const;
            Int integer(Int base = 10) const;
            std::complex<Float> complexValue(Int base = 10) const;

            bool isValue(Int base = 10) const;
            bool isComplex(Int base = 10) const;
            bool isInteger(Int base = 10) const;
        };


        ///Default string constructor
        template <typename CHAR>
        string<CHAR>::string()
        {
            current_length = 1;
            string_array = new CHAR[1];
            string_array[0] = null;
        }

        ///Destructor
        template <typename CHAR>
        string<CHAR>::~string()
        {
            if (string_array)
                delete[] string_array;
        }

        ///Move constructor
        template <typename CHAR>
        string<CHAR>::string(string<CHAR>&& other)
        {
            //snag other's resources and clear other
            current_length = other.current_length;
            string_array = other.string_array;

            other.string_array = NULL;
            other.current_length = 0;
        }

        ///Constructor from null-terminated character string
        template <typename CHAR>
        string<CHAR>::string(const char* buffer)
        {
			if (buffer)
            {
                current_length = 0;

                while (buffer[current_length] != null)
                    current_length++;

                current_length++;

                string_array = new CHAR[current_length];

                for (Int i=0; i<current_length; i++)
                    string_array[i] = buffer[i];

            }
            else
                string();
        }

		template <typename CHAR>
        string<CHAR>::string(const wchar_t* buffer)
        {
            if (buffer)
            {
                current_length = 0;

                while (buffer[current_length] != null)
                    current_length++;

                current_length++;

                string_array = new wchar_t[current_length];

                for (Int i=0; i<current_length; i++)
                    string_array[i] = buffer[i];

            }
            else
                string();
        }

		template <>
        string<char>::string(const wchar_t* buffer)
        {
            if (buffer)
            {
                current_length = 0;

                while (buffer[current_length] != null)
                    current_length++;

                current_length++;

                string_array = new char[current_length];


                Int buf_i = 0;

                for (Int i=0; i<current_length; i++)
                {
                    if (buffer[buf_i] < 128)
                        string_array[i] = buffer[buf_i];
                    else
                    {
                        string_array[i] = '?';

                        if ((buffer[buf_i] >= 0xD800) &&
                            (buffer[buf_i] <= 0xD8FF))
                            buf_i++;
                    }

                    buf_i++;
                }
            }
            else
                string();
        }

        ///Copy constructor from same type
        template <typename CHAR>
        string<CHAR>::string(const string<CHAR>& other)
        {
            current_length = other.current_length;

            string_array = new CHAR[current_length];

            for (Int i=0; i<current_length; i++)
                string_array[i] = other.string_array[i];
        }

        ///Copy constructor from other type
        template <typename CHAR>
        template <typename CHAR_2>
        string<CHAR>::string(const string<CHAR_2>& other)
        {
            current_length = other.length()+1;

            string_array = new CHAR[current_length];

            if (sizeof(CHAR_2) <= sizeof(CHAR))
            {
                for (Int i=0; i<current_length-1; i++)
                    string_array[i] = other.str()[i];

                string_array[current_length-1] = 0;
            }
            else
            {
                Int buf_i = 0;

                for (Int i=0; i<current_length-1; i++)
                {
                    if (other.str()[buf_i] < 128)
                        string_array[i] = other.str()[buf_i];
                    else
                    {
                        string_array[i] = (CHAR)'?';

                        if ((other.str()[buf_i] >= 0xD800) &&
                            (other.str()[buf_i] <= 0xD8FF))
                        buf_i++;
                    }

                    buf_i++;
                }

                string_array[current_length-1] = 0;
            }
        }

        ///Constructor from single narrow character
        template <typename CHAR>
        string<CHAR>::string(const char& character)
        {
            current_length = 2;
            string_array = new CHAR[2];

            string_array[0] = character;

            string_array[1] = null;
        }

        ///Constructor from single wide character
        template <typename CHAR>
        string<CHAR>::string(const wchar_t& character)
        {
            current_length = 2;
            string_array = new CHAR[2];

            if ((sizeof(CHAR) > sizeof(wchar_t)) &&
                (character >= 128))
                string_array[0] = (CHAR)'?';
            else
                string_array[0] = character;

            string_array[1] = null;
        }

        /**
         * \brief Constructor from numeric type
         *
         * If the parameter is any numeric type other than
         * \b char or \b wchar_t, then it is assumed to be
         * a real number, which is then converted Into a string.
         *
         * \param number a value in any numeric type.
         */
        template <typename CHAR>
        template<typename T, typename>
        string<CHAR>::string(const T& number, Int base)
        {
            //buffer assumed to be AT LEAST (2*num_bufsiz + 7) characters long!
            CHAR buffer[num_bufsiz + num_bufsiz + 7];

            current_length = num_to_cstring(number, buffer, base) + 1;

            string_array = new CHAR[current_length];

            for (Int i=0; i<current_length; i++)
                string_array[i] = buffer[i];
        }

		template <typename CHAR>
        template<typename T>
        string<CHAR>::string(T* pointer)
        {
            //buffer assumed to be AT LEAST (2*num_bufsiz + 7) characters long!
            CHAR buffer[num_bufsiz + num_bufsiz + 7];

            current_length = ptr_to_cstring(pointer, buffer) + 1;

            string_array = new CHAR[current_length];

            for (Int i=0; i<current_length; i++)
                string_array[i] = buffer[i];
        }

        /**
         * \brief Constructor from complex numeric type
         *
         * If the parameter is of type  \b std::complex, then
         * it is converted to a string with the format \b a+bi.
         * Note that if either the real or imaginary part are
         * zero, then only the non-zero part is converted. If
         * both parts are zero, then the resultant string is
         * \b "0".
         *
         * \param number a complex number.
         */
        template<typename CHAR>
        template<typename T, typename>
        string<CHAR>::string(const std::complex<T>& number, Int base)
        {
            if (number.imag() == 0)
            {
                //buffer assumed to be AT LEAST (2*num_bufsiz + 7) characters long!
                CHAR buffer[num_bufsiz + num_bufsiz + 7];

                current_length = num_to_cstring((Float)number.real(), buffer, base) + 1;

                string_array = new CHAR[current_length];

                for (Int i=0; i<current_length; i++)
                    string_array[i] = buffer[i];
            }
            else if (number.real() == 0)
            {
                //buffer assumed to be AT LEAST (2*num_bufsiz + 7) characters long!
                CHAR buffer[num_bufsiz + num_bufsiz + 8];

                current_length = num_to_cstring((Float)number.imag(), buffer, base) + 2;
                //append 'i', since imaginary value.
                buffer[current_length-2] = (CHAR)105;
                buffer[current_length-1] = (CHAR)0;

                string_array = new CHAR[current_length];

                for (Int i=0; i<current_length; i++)
                    string_array[i] = buffer[i];
            }
            else
            {
                //each buffer assumed to be AT LEAST (2*num_bufsiz + 7) characters long!
                CHAR real_buffer[num_bufsiz + num_bufsiz + 8];
                CHAR imag_buffer[num_bufsiz + num_bufsiz + 8];

                Int r_array_len = num_to_cstring((Float)number.real(), real_buffer, base) + 1;

                //append '+'(if positive imaginary part),
                //since imaginary value comes next.
                if (number.imag() >= 0)
                    real_buffer[r_array_len-1] = (CHAR)43;
                else
                    r_array_len--;


                Int i_array_len = num_to_cstring((Float)number.imag(), imag_buffer, base) + 2;
                //append 'i', since imaginary value.
                imag_buffer[i_array_len-2] = (CHAR)105;
                imag_buffer[i_array_len-1] = (CHAR)0;

                current_length = r_array_len + i_array_len;
                string_array = new CHAR[current_length];

                for (Int i=0; i<r_array_len; i++)
                    string_array[i] = real_buffer[i];

                for (Int i=0; i<i_array_len; i++)
                    string_array[i+r_array_len] = imag_buffer[i];
            }
        }


        /**
         * \brief String assignment operator from same type.
         */
        template <typename CHAR>
        const string<CHAR>& string<CHAR>::operator=(const string<CHAR>& other)
        {
            if (string_array)
                delete[] string_array;

            current_length = other.current_length;

            string_array = new CHAR[current_length];

            for (Int i=0; i<current_length; i++)
                string_array[i] = other.string_array[i];

            return *this;
        }

        /**
         * \brief String assignment operator from other type.
         */
        template <typename CHAR>
        template <typename CHAR_2>
        const string<CHAR>& string<CHAR>::operator=(const string<CHAR_2>& other)
        {
            if (string_array)
                delete[] string_array;

            current_length = other.length()+1;

            string_array = new CHAR[current_length];

            if (sizeof(CHAR_2) <= sizeof(CHAR))
            {
                for (Int i=0; i<current_length; i++)
                    string_array[i] = other.str()[i];
            }
            else
            {
                Int buf_i = 0;

                for (Int i=0; i<current_length-1; i++)
                {
                    if (other.str()[buf_i] < 128)
                        string_array[i] = other.str()[buf_i];
                    else
                    {
                        string_array[i] = (CHAR)'?';

                        if ((other.str()[buf_i] >= 0xD800) &&
                            (other.str()[buf_i] <= 0xD8FF))
                        buf_i++;
                    }

                    buf_i++;
                }
            }

            return *this;
        }


        /**
         * \brief String equality operator.
         */
        template <typename CHAR>
        inline bool string<CHAR>::operator==(const string<CHAR>& other) const
        {
            return (lessthan_equal_greater(other.string_array,
                                            other.current_length) == 0);
        }

        /**
         * \brief String inequality operator.
         */
        template <typename CHAR>
        inline bool string<CHAR>::operator!=(const string<CHAR>& other) const
        {
            return (lessthan_equal_greater(other.string_array,
                                            other.current_length) != 0);
        }

        /**
         * \brief String greater-than operator.
         */
        template <typename CHAR>
        inline bool string<CHAR>::operator>(const string<CHAR>& other) const
        {
            return (lessthan_equal_greater(other.string_array,
                                            other.current_length) == 1);
        }

        /**
         * \brief String greater-than-or-equal operator.
         */
        template <typename CHAR>
        inline bool string<CHAR>::operator>=(const string<CHAR>& other) const
        {
            return (lessthan_equal_greater(other.string_array,
                                            other.current_length) != -1);
        }

        /**
         * \brief String less-than operator.
         */
        template <typename CHAR>
        inline bool string<CHAR>::operator<(const string<CHAR>& other) const
        {
            return (lessthan_equal_greater(other.string_array,
                                            other.current_length) == -1);
        }

        /**
         * \brief String less-than-or-equal operator.
         */
        template <typename CHAR>
        inline bool string<CHAR>::operator<=(const string<CHAR>& other) const
        {
            return (lessthan_equal_greater(other.string_array,
                                            other.current_length) != -1);
        }

        /**
         * \brief Operator for concatenating to this string.
         */
        template <typename CHAR>
        inline const string<CHAR>& string<CHAR>::operator+=
                                    (const string<CHAR>& other)
        {
            append_string(other.string_array, other.current_length);

            return *this;
        }

        /**
         * \brief Addition operator for returning a concatenated string.
         */
        template <typename CHAR>
        inline const string<CHAR> string<CHAR>::operator+
                                    (const string<CHAR>& other) const
        {
            string<CHAR> output = *this;
            output += other;

            return output;
        }

        /**
         * \brief Replace portion of the string with another string.
         *
         * Replaces characters in the given range with the given
         * replacement string. Note that the range is \b inclusive.
         *
         * \param start_pos the index of the first character to replace.
         * \param end_pos the index of the last character to replace.
         * \param new_string the new string to substitute in.
         */
        template <typename CHAR>
        inline void string<CHAR>::replace(Int start_pos,
                                          Int end_pos,
                                          const string& new_string)
        {
            replace_sub_string_at_with(start_pos,
                                       end_pos,
                                       new_string.string_array,
                                       new_string.current_length);
        }

        /**
         * \brief Convert string to \b char type.
         */
        template <typename CHAR>
        inline const string<char> string<CHAR>::narrow() const
        {
            return string<char>(*this);
        }

        /**
         * \brief Convert string to \b wchar_t type.
         */
        template <typename CHAR>
        inline const string<wchar_t> string<CHAR>::wide() const
        {
            return string<wchar_t>(*this);
        }

        /**
         * \brief The length of the string.
         *
         * \return The number of characters in the string.
         *
         * \see length() const
         */
        template <typename CHAR>
        Int string<CHAR>::size() const
        {
            Int len = (Int)(current_length)-1;

            if (len < 0)
                len = 0;

            return len;
        }

        /**
         * \brief The length of the string.
         *
         * \return The number of characters in the string.
         *
         * \see size() const
         */
        template <typename CHAR>
        inline Int string<CHAR>::length() const
        {
            return size();
        }

        /**
         * \brief A method to get the address of the current character string.
         *
         * \return A const poInter to the string's character array.
         */
        template <typename CHAR>
        inline const CHAR* string<CHAR>::str() const
        {
            return string_array;
        }

        /**
         * \brief Const function to get the character at the given index.
         *
         * \param index the index of the desired character.
         *
         * \return The character at the given index, if the
         * index is valid. Otherwise, \b 0.
         *
         * \see operator[](Int) const
         */
        template <typename CHAR>
        CHAR string<CHAR>::at(Int index) const
        {
            if ((index < 0) ||
                (index >= (Int)current_length-1))
                return null;
            else
                return string_array[index];
        }

        /**
         * \brief Function to get a reference to the character
         * at the given index.
         *
         * Unlike the \b const version, this function allows
         * the character in the string to be modified.
         *
         * \param index the index of the desired character.
         *
         * \return Reference to the character at the given index,
         * if the index is valid. Otherwise, the \b terminating
         * \b character.
         *
         * \see operator[](Int)
         */
        template <typename CHAR>
        CHAR& string<CHAR>::at(Int index)
        {
            if ((index < 0) ||
                (index >= (Int)current_length-1))
                return string_array[current_length-1];
            else
                return string_array[index];
        }

        /**
         * \brief Const operator to get the character at the given index.
         *
         * Identical behavior to at(Int) const method, but
         * allows character indexing like an array.
         *
         * \param index the index of the desired character.
         *
         * \return The character at the given index, if the
         * index is valid. Otherwise, \b 0.
         *
         * \see at(Int) const
         */
        template <typename CHAR>
        inline CHAR string<CHAR>::operator[](Int index) const
        {
            return at(index);
        }

        /**
         * \brief Operator to get the character at the given index.
         *
         * Identical behavior to at(Int) method, but
         * allows character indexing like an array.
         *
         * \param index the index of the desired character.
         *
         * \return The character at the given index, if the
         * index is valid. Otherwise, the \b terminating
         * \b character.
         *
         * \see at(Int)
         */
        template <typename CHAR>
        inline CHAR& string<CHAR>::operator[](Int index)
        {
            return at(index);
        }


        /**
         * \brief Convert the string to its real number equivalent.
         *
         * When converting a string to a number, alphabetic
         * characters are assumed to represent numbers above \b 9.
         * So the string \b "2EB0" in base \b 16 would convert to
         * \b 11952. Valid numeric strings can have up to a single
         * decimal poInt. A leading '-' is also allowed for negative
         * numbers. Additionally, the character \b 'e' or \b 'E'
         * is assumed to imply some exponent in the given base. This
         * exponent can also be negative (e.g. <B>2.2e-3.1</B>).
         * It is important to note bases above \b 14 (such as
         * hexadecimal) have \b E as a digit, so exponents are only
         * Interpreted for bases less than 15.
         *
         * \param base an optional parameter for choosing the base
         * that the string is assumed to be in.
         *
         * \return The value of the string as it appears to humans.
         * \b 0 if there are any invalid characters.
         */
        template <typename CHAR>
        Float string<CHAR>::value(Int base) const
        {

            Float value = 0.0;

            Int start = 0;
            bool isNegative = false;

            if (string_array[0] == (CHAR)'-') //'-' character
            {
                start = 1;
                isNegative = true;
            }

            bool pastDecimal = false;
            bool pastExponent = false;
            bool exponentLast = false;
            Float fracMult = 1.0;
            Int exponent = 0;
            bool expNegative = false;

            Int length = current_length - 1;

            for (Int i=start; i<length; i++)
            {
                //in the rare case that we encounter a null character
                if (string_array[i] == (CHAR)0)
                    break;

                //only one decimal poInt is allowed.
                //any more, and the string is invalid
                if (string_array[i] == (CHAR)'.')
                {
                    if (pastDecimal)
                    {
                        return 0.0;
                    }

                    pastDecimal = true;
                }

                //if a character is not part of a valid number,
                //the string evaluates to 0.0 and return.
                else if (isNumeric(string_array[i], base))
                {
                    if (pastExponent)
                    {
                        exponent *= base;
                        exponent += numeralValue(string_array[i]);

                        exponentLast = false;
                    }
                    else if (pastDecimal)
                    {
                        fracMult /= (Float)base;
                        value += fracMult * (Float)numeralValue(string_array[i]);
                    }
                    else
                    {
                        value *= (Float)base;
                        value += (Float)numeralValue(string_array[i]);
                    }
                }
                else if ((string_array[i] == (CHAR)'E') ||
                         (string_array[i] == (CHAR)'e'))
                {
                    if (pastExponent)
                        return 0;
                    else
                    {
                        pastExponent = true;
                        exponentLast = true;
                    }
                }
                else if (pastExponent &&
                         (string_array[i] == (CHAR)'-'))
                {
                    if (exponentLast)
                        exponentLast = false;
                    else
                        return 0;

                    expNegative = true;
                }
                else if (string_array[i] == (CHAR)'+')
                {
                    if (exponentLast)
                        exponentLast = false;
                    else
                        return 0;
                }
                else if (string_array[i] != (CHAR)'.')
                {
                    return 0;
                }
            }


            if (exponent)
            {
                long valMult = 1;

                for(Int i=0; i<exponent; i++)
                    valMult *= base;

                if (expNegative)
                    value *= (1 / (Float)valMult);
                else
                    value *= valMult;
            }

            //never output "-0"
            if (value == -0)
                return 0;

            if (isNegative)
            {
                return -value;
            }
            else
            {
                return value;
            }
        }

		/**
         * \brief Convert the string to its integer equivalent.
         *
         * When converting a string to an integer, alphabetic
         * characters are assumed to represent numbers above \b 9.
         * So the string \b "2EB0" in base \b 16 would convert to
         * \b 11952. A leading '-' is allowed for negative
         * integers.
         *
         * \param base an optional parameter for choosing the base
         * that the string is assumed to be in.
         *
         * \return The value of the string as it appears to humans.
         * \b 0 if there are any invalid characters.
         */
        template <typename CHAR>
        Int string<CHAR>::integer(Int base) const
        {

            Int value = 0;

            Int start = 0;
            bool isNegative = false;

            if (string_array[0] == (CHAR)'-') //'-' character
            {
                start = 1;
                isNegative = true;
            }


            Int length = current_length - 1;

            for (Int i=start; i<length; i++)
            {
                //in the rare case that we encounter a null character
                if (string_array[i] == (CHAR)0)
                    break;

                //if a character is not part of a valid number,
                //the string evaluates to 0 and return.
                if (isNumeric(string_array[i], base))
                {
                    value *= (Int)base;
                    value += (Int)numeralValue(string_array[i]);
                }
                else
                    return 0;
            }


            if (isNegative)
            {
                return -value;
            }
            else
            {
                return value;
            }
        }

		/**
		 * \brief Convert the string to its complex number equivalent.
		 *
		 * This function splits a numeric string Into its real and
		 * imaginary parts according to \b a+bi or \b a-bi, where
		 * the imaginary part is assumed to come after the real part.
		 * If no imaginary part is detected, then it is assumed to be
		 * \b 0. Also, note that if the base is greater than \b 17,
		 * then the complex part is assumed to be \b 0 as well,
		 * because \b I is a digit in that case.
		 *
		 * \param base an optional parameter for choosing the base
		 * that the string is assumed to be in.
		 *
		 * \return The complex value of the string as it appears to
		 * humans. <B>0+0i</B> if there are any invalid characters.
		 *
		 * \see value(Int) const
		 */
		template <typename CHAR>
		std::complex<Float> string<CHAR>::complexValue(Int base) const
		{
			if (current_length-1 == 0)
				return std::complex<Float>(0,0);

			Float v_real = 0;
			Float v_imag = 0;

			if (base <= 17) //bases >17 have 'i' as a digit
			{
				const string<CHAR> imag_s ('i');

				Int index = this->findLast(imag_s);

				if (index >= 0) //have a complex part
				{
					Int max = current_length-2;

					//real part comes after imaginary
					if ((index < max) &&
						((string_array[index+1] == '+') ||
						(string_array[index+1] == '-')))
					{
						if (string_array[index+1] == '-')
							v_real = this->substr(index+1, max).value(base);
						else
							v_real = this->substr(index+2, max).value(base);

						if (index)
							v_imag = this->substr(0, index).value(base);
						else
							v_imag = 1;
					}
					else //imaginary part comes after real part
					{
						const string<CHAR> imag_plus  ('+');
						const string<CHAR> imag_minus ('-');

						Int signLoc = this->findAfter(imag_plus, 1);
						if (signLoc < 0) signLoc = this->findAfter(imag_minus, 1);

						if (signLoc < 0) //no real part
						{
							if (index == 0)
								v_imag = 1;
							else
								v_imag = this->substr(0, index-1).value(base);
						}
						else //has a real part
						{
							if (index == signLoc + 1)
							{
								if (string_array[signLoc] == '-')
									v_imag = -1;
								else
									v_imag = 1;
							}
							else
							{
								if (string_array[signLoc] == '-')
									v_imag = this->substr(signLoc, index-1).value(base);
								else
									v_imag = this->substr(signLoc+1, index-1).value(base);
							}

							v_real = this->substr(0, signLoc-1).value(base);
						}
					}
				}
				else //no complex part
				{
					v_real = this->value(base);
				}

			}
			else //no complex part
			{
				v_real = this->value(base);
			}

			return std::complex<Float>(v_real, v_imag);
		}


        /**
         * \brief Check if this string can be converted to a real number.
         *
         * When converting a string to a number, alphabetic
         * characters are assumed to represent numbers above \b 9.
         * So the string \b "2EB0" in base \b 16 would convert to
         * \b 11952. Valid numeric strings can have up to a single
         * decimal poInt. A leading '-' is also allowed for negative
         * numbers. Additionally, the character \b 'e' or \b 'E'
         * is assumed to imply some exponent in the given base. This
         * exponent can also be negative (e.g. <B>2.2e-3.1</B>).
         * It is important to note bases above \b 14 (such as
         * hexadecimal) have \b E as a digit, so exponents are only
         * Interpreted for bases less than 15.
         *
         * \param base an optional parameter for choosing the base
         * that the string is assumed to be in.
         *
         * \return \b True if the string contains only characters valid
         * for conversion Into a value. \b False otherwise.
         */
        template <typename CHAR>
        bool string<CHAR>::isValue(Int base) const
        {
            Int start = 0;
            // bool isNegative = false;

            if (string_array[0] == (CHAR)'-') //'-' character
            {
                start = 1;
                // isNegative = true;
            }

            bool pastDecimal = false;
            bool pastExponent = false;
            bool exponentLast = false;
            // bool expNegative = false;

            Int length = current_length - 1;

            for (Int i=start; i<length; i++)
            {
                //in the rare case that we encounter a null character
                if (string_array[i] == (CHAR)0)
                    break;

                //only one decimal poInt is allowed.
                //any more, and the string is invalid
                if (string_array[i] == (CHAR)'.')
                {
                    if (pastDecimal)
                    {
                        return false;
                    }

                    pastDecimal = true;
                }

                //if a character is not part of a valid number,
                //the string evaluates to 0.0 and return.
                else if (isNumeric(string_array[i], base))
                {
                    if (pastExponent)
                    {
                        exponentLast = false;
                    }
                }
                else if ((string_array[i] == (CHAR)'E') ||
                         (string_array[i] == (CHAR)'e'))
                {
                    if (pastExponent)
                        return false;
                    else
                    {
                        pastExponent = true;
                        exponentLast = true;
                    }
                }
                else if (pastExponent &&
                         (string_array[i] == (CHAR)'-'))
                {
                    if (exponentLast)
                        exponentLast = false;
                    else
                        return false;

                    // expNegative = true;
                }
                else if (string_array[i] == (CHAR)'+')
                {
                    if (exponentLast)
                        exponentLast = false;
                    else
                        return false;
                }
                else if (string_array[i] != (CHAR)'.')
                {
                    return false;
                }
            }

            return true;
        }

		/**
         * \brief Check if this string can be converted to an integer.
         *
         * When converting a string to an integer, alphabetic
         * characters are assumed to represent numbers above \b 9.
         * So the string \b "2EB0" in base \b 16 would convert to
         * \b 11952. A leading '-' is allowed for negative
         * integers.
         *
         * \param base an optional parameter for choosing the base
         * that the string is assumed to be in.
         *
         * \return \b True if the string contains only characters valid
         * for conversion into an integer. \b False otherwise.
         */
        template <typename CHAR>
        bool string<CHAR>::isInteger(Int base) const
        {
            Int start = 0;

            if (string_array[0] == (CHAR)'-')
				start = 1;


            Int length = current_length - 1;

            for (Int i=start; i<length; i++)
            {
                //in the rare case that we encounter a null character
                if (string_array[i] == (CHAR)0)
                    break;

                //if a character is not part of a valid number
                else if (!isNumeric(string_array[i], base))
                    return false;
            }

            return (bool)length; //null string is not a number
        }

        /**
         * \brief Check if this string can be converted to a complex number.
         *
         * This function checks if a string can be converted to a complex
         * number of the forms `a+bi` or `ai+b`. Both `a` and `b` may be negative,
		 * and if either the real or imaginary part is not found then it
		 * is assumed to be \b 0.
		 * Also, note that if the base is greater than \b 17,
         * then the complex part is assumed to be \b 0 as well,
         * because \b I is a digit in that case.
         *
         * \param base an optional parameter for choosing the base
         * that the string is assumed to be in.
         *
         * \return \b True if the string contains only characters valid
         * for conversion Into a complex number, and is not null. \b False otherwise.
         *
         * \see isValue(Int) const
         */
		template <typename CHAR>
		bool string<CHAR>::isComplex(Int base) const
		{
			if (current_length-1 == 0)
				return false;

			if (base <= 17) //bases >17 have 'i' as a digit
			{
				const string<CHAR> imag_s ('i');

				Int index = this->findLast(imag_s);

				bool b_real = true;
				bool b_imag = true;

				if (index >= 0) //have a complex part
				{
					Int max = current_length-2;

					//real part comes after imaginary
					if ((index < max) &&
						((string_array[index+1] == '+') ||
						(string_array[index+1] == '-')))
					{
						if (string_array[index+1] == '-')
							b_real = this->substr(index+1, max).isValue(base);
						else
							b_real = this->substr(index+2, max).isValue(base);

						if (index)
							b_imag = this->substr(0, index-1).isValue(base);
						else
							b_imag = true;
					}
					else if (index < max) //extra characters at end
					{
						return false;
					}
					else //imaginary part comes after real part
					{
						const string<CHAR> imag_plus  ('+');
						const string<CHAR> imag_minus ('-');

						Int signLoc = this->findAfter(imag_plus, 1);
						if (signLoc < 0) signLoc = this->findAfter(imag_minus, 1);

						if (signLoc < 0) //no real part
						{
							if (index) //not just 'i'
								b_imag = this->substr(0, index-1).isValue(base);
						}
						else //has a real part
						{
							if (index == signLoc + 1)
							{
								b_imag = true;
							}
							else
							{
								if (string_array[signLoc] == '-')
									b_imag = this->substr(signLoc, index-1).isValue(base);
								else
									b_imag = this->substr(signLoc+1, index-1).isValue(base);
							}

							b_real = this->substr(0, signLoc-1).isValue(base);
						}
					}
				}
				else //no complex part
				{
					b_real = this->isValue(base);
				}

				return b_real && b_imag;
			}
			else //no complex part
			{
				return this->isValue(base);
			}
		}


        /**
         * \brief Convert string to uppercase.
         *
         * If any character in the current string is lowercase,
         * then it is substituted for the uppercase version
         * of that character. All other characters are copied
         * unchanged.
         *
         * \return An uppercase copy of the current string.
         *
         * \see lower() const
         */
        template <typename CHAR>
        const string<CHAR> string<CHAR>::upper() const
        {
            string<CHAR> result (*this);

            for (Int i=0; i<result.length(); i++)
                if ((string_array[i] >= (CHAR)97) &&
                    (string_array[i] <= (CHAR)122))
                {
                    result.string_array[i] = string_array[i] - (CHAR)97 + (CHAR)65;
                }

            return result;
        }

        /**
         * \brief Convert string to lowercase.
         *
         * If any character in the current string is uppercase,
         * then it is substituted for the lowercase version
         * of that character. All other characters are copied
         * unchanged.
         *
         * \return A lowercase copy of the current string.
         *
         * \see upper() const
         */
        template <typename CHAR>
        const string<CHAR> string<CHAR>::lower() const
        {
            string<CHAR> result (*this);

            for (Int i=0; i<result.length(); i++)
                if ((string_array[i] >= (CHAR)65) &&
                    (string_array[i] <= (CHAR)90))
                {
                    result.string_array[i] = string_array[i] - (CHAR)65 + (CHAR)97;
                }

            return result;
        }

        /**
         * \brief Find the first occurrence of the given sub-string.
         *
         * \param sub_string the sub-string to locate.
         *
         * \return The index of the sub-string's first occurrence,
         * if found. Otherwise \b -1.
         *
         * \see find(const string&, Int) const
         */
        template <typename CHAR>
        Int string<CHAR>::find(const string<CHAR>& sub_string) const
            {
                for (Int i=0; i<current_length-1; i++)
                {
                    if (found_sub_string_at(i, sub_string.string_array,
                                            sub_string.current_length))
                        return (Int)i;
                }

                return -1;
            }

        /**
         * \brief Find the n<SUP>th</SUP> occurrence of the given sub-string.
         *
         * Starting from the beginning of the string, find the index
         * of the sub-string's n<SUP>th</SUP> occurrence.
         *
         * \param sub_string the sub-string to locate.
         * \param n the number of occurrence to locate.
         *
         * \return The index of the sub-string's n<SUP>th</SUP> occurrence,
         * if found. Otherwise \b -1.
         *
         * \see find(const string&) const
         */
        template <typename CHAR>
        Int string<CHAR>::find(const string<CHAR>& sub_string, Int n) const
        {
            if (n <= 0)
                return -1;

            Int amount = 0;

            for (Int i=0; i<current_length-1; i++)
            {
                if (found_sub_string_at(i, sub_string.string_array,
                                        sub_string.current_length))
                    amount++;

                if (amount >= n)
                    return (Int)i;
            }

            return -1;
        }

        /**
         * \brief Find the first occurrence of the given sub-string
         * after the given index.
         *
         * \param sub_string the sub-string to locate.
         * \param n the starting index.
         *
         * \return The index of the sub-string's first occurrence,
         * if found. Otherwise \b -1.
         */
        template <typename CHAR>
        Int string<CHAR>::findAfter(const string<CHAR>& sub_string, Int n) const
        {
            for (Int i=n; i<current_length-1; i++)
            {
                if (found_sub_string_at(i, sub_string.string_array, sub_string.current_length))
                    return (Int)i;
            }

            return -1;
        }

        /**
         * \brief Find the last occurrence of the given sub-string
         *
         * \param sub_string the sub-string to locate.
         *
         * \return The index of the sub-string's last occurrence,
         * if found. Otherwise \b -1.
         */
        template <typename CHAR>
        Int string<CHAR>::findLast(const string<CHAR>& sub_string) const
        {
            for (Int i=current_length-1-sub_string.length(); i>=0; i--)
            {
                if (found_sub_string_at(i, sub_string.string_array, sub_string.current_length))
                    return (Int)i;
            }

            return -1;
        }


        /**
         * \brief Replace the given sub-string with another string.
         *
         * Replaces the first occurrence of the given sub-string,
         * and replaces it with the given string.
         *
         * \param sub_string the sub-string to search for.
         * \param new_string the string to substitute in.
         *
         * \return \b False if the sub-string wasn't in the
         * original string. \b True otherwise.
         */
        template <typename CHAR>
        bool string<CHAR>::replace(const string<CHAR>& sub_string,
                                   const string<CHAR>& new_string)
        {
            Int found = find(sub_string);

            if (found > -1)
            {
                replace_sub_string_at_with(found,
                                           found + sub_string.current_length - 2,
                                           new_string.string_array,
                                           new_string.current_length - 1);

                return true;
            }

            return false;
        }

        /**
         * \brief Replace the n<SUP>th</SUP> occurrence of the
         * given sub-string with another string.
         *
         * \param sub_string the sub-string to search for.
         * \param new_string the string to substitute in.
         * \param n the numbered occurrence of the sub-string.
         *
         * \return \b False if the sub-string wasn't in the
         * original string. \b True otherwise.
         */
        template <typename CHAR>
        bool string<CHAR>::replace(const string<CHAR>& sub_string, Int n,
                                   const string<CHAR>& new_string)
        {
            Int found = find(sub_string, n);

            if (found > -1)
            {
                replace_sub_string_at_with(found,
                                           found + sub_string.current_length - 2,
                                           new_string.string_array,
                                           new_string.current_length - 1);

                return true;
            }

            return false;
        }

        /**
         * \brief Get the sub-string within the given range.
         *
         * Copies all characters in the given range, inclusive. If the
         * \b stop parameter is less than \b start, then the sub-string is
         * copied in reverse order.
         *
         * \param start_index the index of the first character to copy.
         * \param end_index the index of the last character to copy.
         *
         * \return A string containing the characters from
         * \b start_index to \b end_index, inclusive.
         */
        template <typename CHAR>
        const string<CHAR> string<CHAR>::substr(Int start_index, Int end_index) const
        {
            //make sure start is within bounds
            if (start_index < 0)
                start_index = 0;
            else if (start_index > (Int)current_length-2)
                start_index = (Int)current_length-2;

            //make sure end is within bounds
            if (end_index < 0)
                end_index = 0;
            else if (end_index > (Int)current_length-2)
                end_index = (Int)current_length-2;


            Int direction;
            Int len;

            //check copy direction
            if (start_index > end_index)
            {
                direction = -1; //backward
                len = start_index-end_index + 2;
            }
            else
            {
                direction = 1; //forward
                len = end_index-start_index + 2;
            }


            //accumulate data
            CHAR temp[len];
            for (Int i=0; i<len-1; i++)
            {
                temp[i] = string_array[i*direction+start_index];
            }
            temp[len - 1] = null;

            //assign data
            string<CHAR> sub_string (temp);

            return sub_string;
        }

        /**
         * \brief Count the occurrences of the given sub-string.
         *
         * \param sub_string the sub-string to search for.
         *
         * \return The number of times that sub-string occurs
         * in the full string.
         */
        template <typename CHAR>
        Int string<CHAR>::count(const string<CHAR>& sub_string) const
        {
            Int amount = 0;

            for (Int i=0; i<current_length-1; i++)
            {
                if (found_sub_string_at(i, sub_string.string_array, sub_string.current_length))
                    amount++;
            }

            return amount;
        }

        /**
         * \brief Remove the characters in the given range from
         * the current string.
		 *
		 * Removes all characters from \b start_index to \b end_index, inclusive.
         *
         * \param start_index the index of the first character to remove.
         * \param end_index that index of the last character to remove.
         */
        template <typename CHAR>
        inline void string<CHAR>::remove(Int start_index, Int end_index)
        {
            remove_portion(start_index, end_index);
        }

        /**
         * \brief Check whether the given sub-string can be found at
         * the given position.
         *
         * \param sub_string the sub-string to compare.
         * \param position the index in the full string to check.
         *
         * \return \b True if the given sub-string was found at
         * the given index. \b False otherwise.
         */
        template <typename CHAR>
        bool string<CHAR>::foundAt(const string<CHAR>& sub_string, Int position) const
        {
            if ((position < 0) || (position >= (Int)current_length-1))
                return false;

            for (Int i=0; i<sub_string.current_length-1; i++)
            {
                if (sub_string[i] != string_array[i + position])
                    return false;
            }

            return true;
        }

        /**
         * \brief Check whether the given sub-string ends at the given position.
         *
         * \param sub_string the sub-string to compare.
         * \param position the index in the full string to check.
         *
         * \return \b True if the sub-string was found in the full string
         * and it ends at the given index. \b False otherwise.
         */
        template <typename CHAR>
        bool string<CHAR>::foundEndAt(const string<CHAR>& sub_string, Int position) const
        {
            if ((position < 0) || (position >= (Int)current_length-1))
                return false;

            for (Int i=sub_string.current_length-2; i>=0; i--)
            {
                if (sub_string[i] != string_array[position])
                    return false;

                position--;
            }

            return true;
        }

        /**
         * \brief Check whether the current string begins with the given
         * sub-string.
         *
         * \param sub_string the sub-string to compare.
         *
         * \return \b True if the full string begins with the given
         * sub-string. \b False otherwise.
         */
        template <typename CHAR>
        inline bool string<CHAR>::beginsWith(const string<CHAR>& sub_string) const
        {
            return foundAt(sub_string, 0);
        }

        /**
         * \brief Check whether the current string begins with the given
         * sub-string, but skip over padding characters.
         *
         * \param sub_string the sub-string to search for.
         * \param pad_char the character to skip over.
         *
         * \return \b True if the given sub-string was found at the
         * beginning of the string, after \b 0 or more padding
         * characters. \b False otherwise.
         */
        template <typename CHAR>
        bool string<CHAR>::beginsWith(const string<CHAR>& sub_string, CHAR pad_char) const
        {
            Int i = 0;

            while ((string_array[i] == pad_char) && (i < (Int)current_length-1))
            {
                if (foundAt(sub_string, i))
                    return true;

                i++;
            }

            return foundAt(sub_string, i);
        }


        /**
         * \brief Check whether the current string ends with the given
         * sub-string.
         *
         * \param sub_string the sub-string to compare.
         *
         * \return \b True if the full string ends with the given
         * sub-string. \b False otherwise.
         */
        template <typename CHAR>
        inline bool string<CHAR>::endsWith(const string<CHAR>& sub_string) const
        {
            return foundEndAt(sub_string, current_length-2);
        }

        /**
         * \brief Clear string contents
         *
         */
        template <typename CHAR>
        void string<CHAR>::clear()
        {
            delete[] string_array;

            current_length = 1;
            string_array = new CHAR[1];

            string_array[0] = null;
        }



        //Append a character buffer to the current buffer
        template <typename CHAR>
        void string<CHAR>::append_string(const CHAR* buffer, Int bufsiz)
        {
            if (buffer)
            {
                //shift current data
                CHAR* temp = string_array;
                Int tempsiz = current_length;

                //prep for new data
                current_length = tempsiz + bufsiz - 1;
                string_array = new CHAR[current_length];


                //copy old data
                for (Int i=0; i<tempsiz-1; i++)
                    string_array[i] = temp[i];

                //copy new data
                for (Int j=tempsiz-1; j<current_length; j++)
                    string_array[j] = buffer[j - tempsiz + 1];

                delete[] temp;
            }
        }

        //Append a character to the current buffer
        template <typename CHAR>
        void string<CHAR>::append_char(CHAR singleChar)
        {
            //shift current data
            CHAR* temp = string_array;
            Int tempsiz = current_length;

            if (!string_array || !current_length)
                tempsiz++;

            //prep for new data
            current_length = tempsiz + 1;
            string_array = new CHAR[current_length];

            //copy old data
            for (Int i=0; i<tempsiz-1; i++)
                string_array[i] = temp[i];

            //append new data
            string_array[current_length-2] = singleChar;
            string_array[current_length-1] = null;

            delete[] temp;
        }

        //Check whether the characters in the given buffer
        //are found at the given location.
        template <typename CHAR>
        bool string<CHAR>::found_sub_string_at(Int pos,
                                               const CHAR* substr,
                                               Int substr_len) const
        {
            for (Int i=0 ; i<substr_len-1; i++)
            {
                //if the string ends before all of the substring was found
                if (pos+i >= current_length-1)
                    return false;

                //if the exact substring was not found here
                if (substr[i] != string_array[i+pos])
                    return false;
            }

            return true;
        }

        //Replace the character in the given range (inclusive)
        //with the characters in the given buffer.
        template <typename CHAR>
        void string<CHAR>::replace_sub_string_at_with(Int beg_index,
                                                      Int end_index,
                                                      const CHAR* sub_str,
                                                      Int substr_len)
        {
            if (string_array && current_length)
            {
                //make sure start index is within bounds
                if (beg_index < 0)
                    beg_index = 0;
                else if (beg_index >= (Int)current_length-1)
                    return;

                //make sure end index is within bounds
                if (end_index < 0)
                    return;
                else if (end_index >= (Int)current_length-1)
                    end_index = (Int)current_length-2;

                //make sure start index is not after end index
                if (beg_index > end_index)
                    return;


                //shift the string data
                CHAR* temp = string_array;
                Int tempsiz = current_length;

                //prep data
                current_length = current_length + substr_len - (end_index - beg_index);
                string_array = new CHAR[current_length];

                Int index = 0;
                //copy original data until the start position
                for (Int i=0; i<beg_index; i++)
                {
                    string_array[index] = temp[i];
                    index++;
                }

                //copy the new substring Into place
                for (Int i=0; i<substr_len-1; i++)
                {
                    string_array[index] = sub_str[i];
                    index++;
                }

                //copy original data from the end position
                for (Int i=end_index+1; i<tempsiz; i++)
                {
                    string_array[index] = temp[i];
                    index++;
                }

                string_array[current_length-1] = null;

                delete[] temp;


            }
        }

        //Internal function to check string equality
        //returns -1 if this string is less than another,
        //        +1 if it is greater,
        //     and 0 if they are the same.
        template <typename CHAR>
        Int string<CHAR>::lessthan_equal_greater(const CHAR* other,
                                                 Int str_size) const
        {
            Int i = 0;

            while ((i<str_size-1) && (i<current_length-1))
            {
                if (string_array[i] < other[i])
                    return -1;
                else if (string_array[i] > other[i])
                    return 1;
                else if ((string_array[i] == null) && (other[i] == null))
                    return 0;

                i++;
            }

            if (current_length < str_size)
                return -1;
            else if (current_length > str_size)
                return 1;
            else
                return 0;
        }

        //Internal function to remove a section of text
        //from the string (start+end inclusive)
        template <typename CHAR>
        void string<CHAR>::remove_portion(Int beg_index, Int end_index)
        {
            //make sure start index is within bounds
            if (beg_index < 0)
                beg_index = 0;
            else if (beg_index > (Int)current_length-2)
                beg_index = (Int)current_length-2;

            //make sure end index is within bounds
            if (end_index < 0)
                end_index = 0;
            else if (end_index > (Int)current_length-2)
                end_index = (Int)current_length-2;

            //make sure start index is not after end index
            if (beg_index > end_index)
                return;


            //shift data
            Int tempsiz = current_length;
            CHAR* temp = string_array;

            //prep new string
            Int section = (end_index - beg_index + 1);
            current_length = tempsiz - section;
            string_array = new CHAR[current_length];

            //copy part of old string before removed section
            for (Int i=0; i<beg_index; i++)
                string_array[i] = temp[i];

            //copy part of old string after removed section
            for (Int i=end_index+1; i<(Int)tempsiz; i++)
                string_array[i - section] = temp[i];

            delete[] temp;
        }


    }
}


#undef num_bufsiz
#undef num_precision

#undef num_round_magic

#endif // STRING_H_INCLUDED
