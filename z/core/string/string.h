/**
 * File:            string.h
 * Namespace:       z::core
 *
 * Description:     A template for character strings. The two
 *                  currently supported character types are
 *                  char and wchar_t. Strings can be created with
 *                  'z::core::string<TYPE> foo', where 'TYPE' is
 *                  either char or wchar_t.
 *                  This class is also type-independent, that is
 *                  it handles all type conversions automatically.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   16 Apr. 2017
**/


#pragma once
#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#ifndef null
    #define null 0
#endif // null

//this is for initializing numbers properly
#include <type_traits>


#define num_bufsiz 15
#define num_precision 9


#include "convert_char_type.h"

namespace z
{
    namespace core
    {
        template <typename CHAR>
        class string
        {
        public:
            int array_length;
            CHAR* string_array;

            //internal function to clear string data
            void clear_data()
            {
                if (string_array)
                    delete[] string_array;

                string_array = null;
                array_length = 0;
            }

            //internal function to assign new data
            void assign_data(const CHAR* buffer, int bufsiz)
            {
                clear_data();

                if (buffer)
                {
                    array_length = bufsiz+1;
                    string_array = new CHAR[array_length];

                    for (int i=0; i<array_length-1; i++)
                    {
                        string_array[i] = buffer[i];
                        if (buffer[i] == null)
                        {
                            array_length = i+1;
                            break;
                        }
                    }

                    string_array[array_length-1] = null;
                }
            }

            //internal function to append a character array
            void append_string(const CHAR* buffer, int bufsiz)
            {
                if (buffer)
                {
                    //shift current data
                    CHAR* temp = string_array;
                    int tempsiz = array_length;

                    //prep for new data
                    array_length = tempsiz + bufsiz - 1;
                    string_array = new CHAR[array_length];


                    //copy old data
                    for (int i=0; i<tempsiz-1; i++)
                        string_array[i] = temp[i];

                    //copy new data
                    for (int j=tempsiz-1; j<array_length; j++)
                        string_array[j] = buffer[j - tempsiz + 1];

                    delete[] temp;
                }
            }

            //internal function to append a single character
            void append_char(CHAR singleChar)
            {
                //shift current data
                CHAR* temp = string_array;
                int tempsiz = array_length;

                if (!string_array || !array_length)
                    tempsiz++;

                //prep for new data
                array_length = tempsiz + 1;
                string_array = new CHAR[array_length];

                //copy old data
                for (int i=0; i<tempsiz-1; i++)
                    string_array[i] = temp[i];

                //append new data
                string_array[array_length-2] = singleChar;
                string_array[array_length-1] = null;

                delete[] temp;
            }

            //internal function to check if the specified sub-string is at the specified position
            bool found_sub_string_at(int pos, const CHAR* substr, int substr_len) const
            {
                for (int i=0 ; i<substr_len-1; i++)
                {
                    //if the string ends before all of the substring was found
                    if (pos+i >= array_length-1)
                        return false;

                    //if the exact substring was not found here
                    if (substr[i] != string_array[i+pos])
                        return false;
                }

                return true;
            }

            //internal function to replace the substring in the given index range(inclusive) with the given string
            void replace_sub_string_at_with(int beg_index, int end_index, const CHAR* sub_str, int substr_len)
            {
                if (string_array && array_length)
                {
                    //make sure start index is within bounds
                    if (beg_index < 0)
                        beg_index = 0;
                    else if (beg_index >= (int)array_length-1)
                        return;

                    //make sure end index is within bounds
                    if (end_index < 0)
                        return;
                    else if (end_index >= (int)array_length-1)
                        end_index = (int)array_length-2;

                    //make sure start index is not after end index
                    if (beg_index > end_index)
                        return;


                    //shift the string data
                    CHAR* temp = string_array;
                    int tempsiz = array_length;

                    //prep data
                    array_length = array_length + substr_len - (end_index - beg_index);
                    string_array = new CHAR[array_length];

                    int index = 0;
                    //copy original data until the start position
                    for (int i=0; i<beg_index; i++)
                    {
                        string_array[index] = temp[i];
                        index++;
                    }

                    //copy the new substring into place
                    for (int i=0; i<substr_len-1; i++)
                    {
                        string_array[index] = sub_str[i];
                        index++;
                    }

                    //copy original data from the end position
                    for (int i=end_index+1; i<tempsiz; i++)
                    {
                        string_array[index] = temp[i];
                        index++;
                    }

                    string_array[array_length-1] = null;

                    delete[] temp;


                }
            }

            //internal function to check string equality
            //returns -1 if this string is less than another,
            //        +1 if it is greater,
            //     and 0 if they are the same.
            int lessthan_equal_greater(const CHAR* other, int str_size) const
            {
                int i = 0;

                while ((i<str_size-1) && (i<array_length-1))
                {
                    if (string_array[i] < other[i])
                        return -1;
                    else if (string_array[i] > other[i])
                        return 1;
                    else if ((string_array[i] == null) && (other[i] == null))
                        return 0;

                    i++;
                }

                if (array_length < str_size)
                    return -1;
                else if (array_length > str_size)
                    return 1;
                else
                    return 0;
            }


            //internal function to remove a section of text from the string (start+end inclusive)
            void remove_portion(int beg_index, int end_index)
            {
                //make sure start index is within bounds
                if (beg_index < 0)
                    beg_index = 0;
                else if (beg_index > (int)array_length-2)
                    beg_index = (int)array_length-2;

                //make sure end index is within bounds
                if (end_index < 0)
                    end_index = 0;
                else if (end_index > (int)array_length-2)
                    end_index = (int)array_length-2;

                //make sure start index is not after end index
                if (beg_index > end_index)
                    return;


                //shift data
                int tempsiz = array_length;
                CHAR* temp = string_array;

                //prep new string
                int section = (end_index - beg_index + 1);
                array_length = tempsiz - section;
                string_array = new CHAR[array_length];

                //copy part of old string before removed section
                for (int i=0; i<beg_index; i++)
                    string_array[i] = temp[i];

                //copy part of old string after removed section
                for (int i=end_index+1; i<(int)tempsiz; i++)
                    string_array[i - section] = temp[i];

                delete[] temp;
            }


        public:
            //empty constructor
            string() {array_length = 1; string_array = new CHAR[1]; string_array[0] = null;}

            //default constructor
            string(const CHAR* buffer)
            {
                array_length = 0;
                string_array = null;

                int amount = 0;

                if (buffer)
                {
                    while (buffer[amount] != null)
                        amount++;

                    assign_data(buffer, amount++);
                }
            }

            template <typename CHAR_2>
            //constructor using other chars
            string(const CHAR_2* buffer)
            {
                CHAR_2* old_buffer = (CHAR_2*)((void*)buffer);

                array_length = 0;
                string_array = null;

                if (old_buffer)
                {
                    int len = 0;

                    while (old_buffer[len] != null)
                        len++;

                    CHAR* new_buffer = null;

                    convertStr(new_buffer, old_buffer, len+1);

                    array_length = len+1;
                    string_array = new_buffer;
                }
            }


            //copy constructor
            string(const string& other)
            {
                string_array = null;
                array_length = 0;

                assign_data(other.string_array, other.array_length);
            }


            template <typename CHAR_2>
            //copy constructor for other types
            string(const string<CHAR_2>& other)
            {
                string_array = null;
                array_length = 0;


                CHAR_2* other_ptr = (CHAR_2*)((void*)other.str());
                int length = other.length();

                CHAR* this_ptr = null;


                convertStr(this_ptr, other_ptr, length + 1);

                string_array = this_ptr;

                array_length = length + 1;
            }


            //narrow char constructor
            string(char character)
            {
                array_length = 2;
                string_array = new CHAR[2];

                convertChr(string_array[0], character);
                string_array[1] = null;
            }

            //wide char constructor
            string(wchar_t character)
            {
                array_length = 2;
                string_array = new CHAR[2];

                convertChr(string_array[0], character);
                string_array[1] = null;
            }


            //numeric constructor
            ///converts a number to a string
            template<
                typename T, //real type
                typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
            >
            string(T number)
            {
                string_array = null;
                array_length = 0;


                CHAR buffer[num_bufsiz + num_bufsiz + 3];
                int buffer_pos = 0;

                long ipart = number;
                double fpart = number - ipart;

                CHAR inv_ibuf[num_bufsiz];
                CHAR inv_fbuf[num_bufsiz];

                int ibufsiz = 0;
                int fbufsiz = 0;


                if (number < 0)
                {
                    buffer[buffer_pos] = (CHAR)45;
                    buffer_pos++;
                }


                if (ipart == 0)
                {
                    buffer[buffer_pos] = (CHAR)48;
                    buffer_pos++;
                }
                else
                {
                    while (ipart != 0)
                    {
                         inv_ibuf[ibufsiz] = (CHAR)((ipart % 10) + 48);
                         ipart /= 10;

                         ibufsiz++;
                    }


                    while ((fpart != 0) && (fbufsiz < num_precision))
                    {
                        fpart *= 10;

                        int frac_char = fpart;

                        inv_fbuf[fbufsiz] = (CHAR)(frac_char + 48);

                        fbufsiz++;
                        fpart -= frac_char;
                    }
                }


                for (int i=ibufsiz-1; i>=0; i--)
                {
                    buffer[buffer_pos] = inv_ibuf[i];
                    buffer_pos++;
                }

                if (fbufsiz)
                {
                    buffer[buffer_pos] = (CHAR)46;
                    buffer_pos++;

                    int frac_len = 0;

                    for (int f=0; f<fbufsiz; f++)
                    {
                        buffer[buffer_pos + f] = inv_fbuf[f];

                        if (inv_fbuf[f] > 48)
                            frac_len = f + 1;
                    }

                    buffer_pos += frac_len;
                }

                //buffer is null-terminated
                buffer[buffer_pos] = (CHAR)0;

                assign_data(buffer, buffer_pos+1);
            }


            //destructor
            ~string()
            {
                if (string_array)
                    delete[] string_array;
            }

            //get number of characters in the string
            int length() const
            {
                int len = (int)(array_length)-1;

                if (len < 0)
                    len = 0;

                return len;
            }

            //get number of bytes the string takes up in memory
            int size() const
            {
                int sum = 0;

                for (int i=0; i<array_length; i++)
                    sum += sizeof(string_array[i]);

                return sum;
            }


            //get string array pointer
            inline const CHAR* str() const {return string_array;}




            //equality operator
            inline bool operator==(const string& other) const
            {
                return (lessthan_equal_greater(other.string_array, other.array_length) == 0);
            }


            //inequality operator
            inline bool operator!=(const string& other) const
            {
                return (lessthan_equal_greater(other.string_array, other.array_length) != 0);
            }


            //greater than operator
            inline bool operator>(const string& other) const
            {
                return (lessthan_equal_greater(other.string_array, other.array_length) == 1);
            }

            //greater than / equal to operator
            inline bool operator>=(const string& other) const
            {
                return (lessthan_equal_greater(other.string_array, other.array_length) != -1);
            }


            //less than operator
            inline bool operator<(const string& other) const
            {
                return (lessthan_equal_greater(other.string_array, other.array_length) == -1);
            }


            //less than / equal to operator
            inline bool operator<=(const string& other) const
            {
                return (lessthan_equal_greater(other.string_array, other.array_length) != -1);
            }


            //operator for concatenation of string
            inline const string& operator+=(const string& other)
            {
                append_string(other.string_array, other.array_length);

                return *this;
            }

            //operator for concatenation of a single character
            /*inline const string& operator+=(CHAR character)
            {
                append_char(character);

                return *this;
            }*/

            //operator for concatenation of a number (convert to string first)
            /*const string& operator+=(double number)
            {
                string String;

                toString(number, String);

                append_string(String.string_array, String.array_length);

                return *this;
            }*/


            //addition operator
            inline const string operator+(const string& other) const
            {
                string output = *this;
                output += other;

                return output;
            }

            //addition operator
            /*const string operator+(const CHAR*& other) const
            {
                string output = *this;
                output += string(other);

                return output;
            }

            //addition operator
            const string operator+(const CHAR other) const
            {
                string output = *this;
                output += string(other);

                return output;
            }*/

            //index operator
            //if the index is valid, returns character at that index.
            //otherwise, returns 0, since no character in the string will be 0.
            CHAR operator[](int index) const
            {
                if (index < 0)
                    return null;
                else if (index >= (int)array_length-1)
                    return null;
                else
                    return string_array[index];
            }


            //function to get character at given index.
            //Identical functionality to this->operator[].
            CHAR at(int index) const
            {
                if (index < 0)
                    return null;
                else if (index >= (int)array_length-1)
                    return null;
                else
                    return string_array[index];
            }

            //function to find the position of the first occurrence of the given sub-string
            //returns -1 if it was not found
            int find(const string& sub_string) const
            {
                for (int i=0; i<array_length-1; i++)
                {
                    if (found_sub_string_at(i, sub_string.string_array, sub_string.array_length))
                        return (int)i;
                }

                return -1;
            }

            //function to find the position of the LAST occurrence of the given sub-string
            //returns -1 if it was not found
            int findLast(const string& sub_string) const
            {
                for (int i=array_length-1-sub_string.length(); i>=0; i--)
                {
                    if (found_sub_string_at(i, sub_string.string_array, sub_string.array_length))
                        return (int)i;
                }

                return -1;
            }

            //function to find the position of the nth occurrence of the given sub-string
            ///n starts at 0
            //returns -1 if it was not found
            int find(const string& sub_string, int n) const
            {
                if (n < 0)
                    return -1;

                int amount = -1;

                for (int i=0; i<array_length-1; i++)
                {
                    if (found_sub_string_at(i, sub_string.string_array, sub_string.array_length))
                        amount++;

                    if (amount >= n)
                        return (int)i;
                }

                return -1;
            }


            //function to replace a sub-string (indexes start_pos to end_pos, inclusive) with some new string
            //no return, as this will always succeed when given correct parameters
            void replace(int start_pos, int end_pos, const string& new_string)
            {
                replace_sub_string_at_with(start_pos, end_pos,
                                           new_string.string_array, new_string.array_length);
            }


            //function to replace the first occurrence of the first given sub-string with the second one.
            //returns false if the sub-string wasn't in the original string, true otherwise.
            bool replace(const string& sub_string, const string& new_string)
            {
                int found = find(sub_string);

                if (found > -1)
                {
                    replace_sub_string_at_with(found, found + sub_string.array_length - 2,
                                               new_string.string_array, new_string.array_length - 1);

                    return true;
                }

                return false;
            }


            //function to replace the nth occurrence of the first given sub-string with the second one.
            ///n starts at 0
            //returns false if the sub-string wasn't in the original string, true otherwise.
            bool replace(const string& sub_string, int n, const string& new_string)
            {
                int found = find(sub_string, n);

                if (found > -1)
                {
                    replace_sub_string_at_with(found, found + sub_string.array_length - 2,
                                               new_string.string_array, new_string.array_length - 1);

                    return true;
                }

                return false;
            }


            //function to return a substring given two positions
            const string substr(int start_index, int end_index) const
            {
                //make sure start is in bounds
                if (start_index < 0)
                    start_index = 0;
                else if (start_index > (int)array_length-2)
                    return string();

                //make sure end is in bounds
                if (end_index < 0)
                    return string();
                else if (end_index > (int)array_length-2)
                    end_index = (int)array_length-2;

                //start must come before end
                if (start_index > end_index)
                    return string();


                //accumulate data
                int len = end_index-start_index + 2;
                CHAR temp[len];
                for (int i=0; i<len-1; i++)
                    temp[i] = string_array[i + start_index];
                temp[len - 1] = null;

                //assign data
                string sub_string;
                sub_string.assign_data(temp, len);

                return sub_string;
            }


            //function to return the number of times the given string appears in this string
            int count(const string& sub_string) const
            {
                int amount = 0;

                for (int i=0; i<array_length-1; i++)
                {
                    if (found_sub_string_at(i, sub_string.string_array, sub_string.array_length))
                        amount++;
                }

                return amount;
            }


            //function to remove the portion of the string within the specified indexes (inclusive)
            void remove(int start_index, int end_index)
            {
                remove_portion(start_index, end_index);
            }


            //returns true if the specified sub-string was found at the specified position
            bool foundAt(const string& sub_string, int position) const
            {
                if ((position < 0) || (position >= (int)array_length-1))
                    return false;

                for (int i=0; i<sub_string.array_length-1; i++)
                {
                    if (sub_string[i] != string_array[i + position])
                        return false;
                }

                return true;
            }

            //returns true if the specified sub-string was found at
            //AND ENDS AT the specified position
            bool foundEndAt(const string& sub_string, int position) const
            {
                if ((position < 0) || (position >= (int)array_length-1))
                    return false;

                for (int i=sub_string.array_length-2; i>=0; i--)
                {
                    if (sub_string[i] != string_array[position])
                        return false;

                    position--;
                }

                return true;
            }

            //returns true if specified text was found at the beginning of the string
            inline bool beginsWith(const string& sub_string) const
            {
                return foundAt(sub_string, 0);
            }

            //returns true if specified text was found at the end of the string
            inline bool endsWith(const string& sub_string) const
            {
                return foundEndAt(sub_string, array_length-2);
            }

            //returns true if specified text was found at the beginning of the string,
            //after an allowed padding char (e.g. leading spaces)
            bool beginsWith(const string& sub_string, CHAR pad_char) const
            {
                int i = 0;

                while ((string_array[i] == pad_char) && (i < (int)array_length-1))
                {
                    if (foundAt(sub_string, i))
                        return true;

                    i++;
                }

                return foundAt(sub_string, i);
            }


            ///Clears the string
            void clear()
            {
                clear_data();

                array_length = 1;
                string_array = new CHAR[1];

                string_array[0] = null;
            }

            const string& operator=(const string& other)
            {
                assign_data(other.string_array, other.length());

                return *this;
            }

            template <typename CHAR_2>
            const string& operator=(const string<CHAR_2>& other)
            {
                CHAR_2* other_ptr = (CHAR_2*)((void*)other.str());
                int length = other.length();

                CHAR* this_ptr = null;


                convertStr(this_ptr, other_ptr, length + 1);


                delete[] string_array;

                string_array = this_ptr;

                array_length = length + 1;


                return *this;
            }
        };

    }
}


#undef num_bufsiz
#undef num_precision

#endif // STRING_H_INCLUDED
