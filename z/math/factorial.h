#pragma once
#ifndef FACTORIAL_H_INCLUDED
#define FACTORIAL_H_INCLUDED

#include <cmath>

#define FACTORIAL_MAX_INPUT_INT 21

namespace z
{
    namespace math
    {
        /**
         * \brief Calculate the factorial of the integer \b n.
         *
         * \param n a positive integer.
         *
         * \return The factorial of \b n, if <B>n>0</B> and
         * the factorial is able to be represented. \b 0 otherwise.
         */
        template<typename INT,
            typename = typename std::enable_if<std::is_integral<INT>::value>::type>
        INT factorial(INT n)
        {
            if (n < 0)
                return 0;


            INT result = 1;

            INT i = 2;
            while (i <= n)
            {
                INT newResult = result * i;

                if ((newResult == result) ||
                    (newResult <= 0))
                    return 0;

                result = newResult;
                i++;
            }

            return result;
        }

        template<typename INT,
            typename = typename std::enable_if<std::is_integral<INT>::value>::type>
        INT factorial(INT n, bool& did_overflow)
        {
            if (n < 0)
            {
                did_overflow = false;
                return 0;
            }


            INT result = 1;

            INT i = 2;
            while (i <= n)
            {
                INT newResult = result * i;

                if ((newResult == result) ||
                    (newResult <= 0))
                {
                    did_overflow = true;
                    return 0;
                }

                result = newResult;
                i++;
            }

            did_overflow = false;
            return result;
        }
    }
}

#undef FACTORIAL_MAX_INPUT_INT

#endif // FACTORIAL_H_INCLUDED
