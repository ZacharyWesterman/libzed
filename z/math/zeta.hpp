#pragma once

namespace z {
namespace math {
/**
 * @deprecated This function will be removed in `1.9`.
 * It's being removed for 3 reasons that basically boil down to
 * it not being worth the file size it adds to the library.
 * 1. This function is trivial to implement.
 * 2. The Riemann zeta function is rarely needed in practice.
 * 3. This implementation is neither fast nor all that accurate.
 *
 * @brief A function for approximating Reimann-zeta sums
 *
 * Computes zeta(s) with reasonable precision.
 *
 * @param s An integer greater than 1
 *
 * @return An approximation of zeta(s)
 *
 * @threadsafe_function_yes
 */
double zeta(const int s) noexcept;
} // namespace math
} // namespace z
