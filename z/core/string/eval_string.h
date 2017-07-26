/**
 * File:            eval_string.h
 * Namespace:       z::core
 *
 * Description:     A template function that evaluates a string
 *                  and returns the found value.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   26 Jul. 2017
**/

#pragma once
#ifndef EVAL_STRING_H_INCLUDED
#define EVAL_STRING_H_INCLUDED

#include "string.h"
#include "char_is_alphanumeric.h"


namespace z
{
    namespace core
    {
        ///template for string evaluation functions
        // both ignore spaces, and returns 0.0 if non-numerical characters are present
        template <typename CHAR>
        double value(const string<CHAR>& input, int base = 10)
        {

            double value = 0.0;

            int start = 0;
            bool isNegative = false;

            if (input[0] == (CHAR)45) //'-' character
            {
                start = 1;
                isNegative = true;
            }

            bool pastDecimal = false;
            bool pastExponent = false;
            double fracMult = 1.0;
            int exponent = 0;
            bool expNegative = false;

            int length = input.length();

            for (int i=start; i<length; i++)
            {
                //in the rare case that we encounter a null character
                if (input[i] == (CHAR)0)
                    break;

                //only one decimal point is allowed.
                //any more, and the string is invalid
                if (input[i] == (CHAR)46) //'.' character
                {
                    if (pastDecimal)
                    {
                        return 0.0;
                    }

                    pastDecimal = true;
                }

                //if a character is not part of a valid number,
                //the string evaluates to 0.0 and return.
                else if (is_numeric(input[i], base))
                {
                    if (pastExponent)
                    {
                        exponent *= base;
                        exponent += numeral_value(input[i]);
                    }
                    else if (pastDecimal)
                    {
                        fracMult /= (double)base;
                        value += fracMult * (double)numeral_value(input[i]); //actual value from character
                    }
                    else
                    {
                        value *= (double)base;
                        value += (double)numeral_value(input[i]); //actual value from character
                    }
                }
                else if (input[i] == (CHAR)69)
                {
                    pastExponent = true;
                }
                else if (pastExponent && (input[i] == (CHAR)45))
                {
                    expNegative = true;
                }
                else if (input[i] != (CHAR)46)
                {
                    return 0.0;
                }
            }


            if (exponent)
            {
                long valMult = 1;

                for(int i=0; i<exponent; i++)
                    valMult *= base;

                if (expNegative)
                    value *= (1 / (double)valMult);
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


        template <typename CHAR>
        std::complex<double> complexValue(const string<CHAR>& input, int base = 10)
        {
            if (input.endsWith("i"))
            {
                double real(0), imag;

                int realEnd = input.findLast("+");

                imag = value(input.substr(realEnd+1, input.length()-2), base);

                if (realEnd >= 0)
                {
                    real = value(input.substr(0, realEnd-1), base);
                }
                else
                {
                    realEnd = input.findLast("-");

                    if (realEnd >= 0)
                    {
                        real = value(input.substr(0, realEnd-1), base);
                        imag = -value(input.substr(realEnd+1, input.length()-2), base);
                    }
                }

                return std::complex<double> (real, imag);
            }
            else
            {
                return std::complex<double> (value(input, base), 0);
            }

        }
    }
}

#endif // EVAL_STRING_H_INCLUDED
