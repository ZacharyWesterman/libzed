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
 * Last modified:   2 Feb. 2017
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
        double value(const string<CHAR>& input)
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
            double fracMult = 1.0;

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
                else if (is_numeric(input[i]))
                {
                    if (pastDecimal)
                    {
                        fracMult /= 10.0;
                        value += fracMult * (double)(input[i] - (CHAR)48); //actual value from character
                    }
                    else
                    {
                        value *= 10.0;
                        value += (double)(input[i] - (CHAR)48); //actual value from character
                    }
                }
                else if (input[i] != (CHAR)46)
                {
                    return 0.0;
                }
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

    }
}

#endif // EVAL_STRING_H_INCLUDED
