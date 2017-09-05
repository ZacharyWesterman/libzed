#pragma once
#ifndef ZETA_H_INCLUDED
#define ZETA_H_INCLUDED

#include <z/float.h>

namespace z
{
    namespace math
    {
        /**
         * \fn Float zeta(const int s)
         * \param s: an integer greater than 1
         * \return An approximation of zeta(s)
         *
         * \brief A function for approximating Reimann-zeta sums
         *
         * Computes zeta(s) with the highest possible precision for
         * the float type used. Note that this approximation is
         * less accurate for single-precision floats.
         */
        Float zeta(const int s)
        {
            Float result = 1.0;

            int i = 2;


            #ifdef Z_USE_DOUBLE //doubles can cause this computation to take too long.
            while (i<100000)
            {
            #else
            Float oldResult = -1.0;
            while (result != oldResult)
            {
                oldResult = result;
            #endif // Z_USE_DOUBLE
                Float denomP = 1.0 / (Float)i;

                Float partial = 1;
                for (int j=0; j<s; j++)
                    partial *= denomP;

                result += partial;

                i++;
            }


            return result;
        }
    }
}

#endif // ZETA_H_INCLUDED
