#pragma once
#ifndef ZETA_H_INCLUDED
#define ZETA_H_INCLUDED

namespace z
{
    namespace math
    {
        /**
         * \fn zFloat zeta(const int s)
         * \param s: an integer greater than 1
         * \return An approximation of zeta(s)
         *
         * \brief A function for approximating Reimann-zeta sums
         *
         * Computes zeta(s) with the highest possible precision for
         * the float type used. Note that this approximation is
         * less accurate for single-precision floats.
         */
        zFloat zeta(const int s)
        {
            zFloat result = 1.0;

            int i = 2;


            #ifdef Z_USE_DOUBLE //doubles can cause this computation to take too long.
            while (i<100000)
            {
            #else
            zFloat oldResult = -1.0;
            while (result != oldResult)
            {
                oldResult = result;
            #endif // Z_USE_DOUBLE
                zFloat denomP = 1.0 / (zFloat)i;

                zFloat partial = 1;
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
