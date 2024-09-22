#pragma once

#include <complex>

/**
 * @brief Calculate the remainder of complex division \b x/y.
 *
 * Performs integer division on complex numbers \b x and \b y to calculate a remainder.
 *
 * @param x the numerator.
 * @param y the denominator.
 *
 * @return The remainder of complex integer division \b x/y.
 *
 * @threadsafe_function_yes
 */
std::complex<long> operator%(const std::complex<long> &x, const std::complex<long> &y) noexcept;
