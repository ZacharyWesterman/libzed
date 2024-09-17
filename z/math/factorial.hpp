#pragma once

namespace z {
namespace math {
/**
 * \brief Calculate the factorial of the integer \b n.
 * Does not check for overflow.
 *
 * \param n a positive integer.
 *
 * \return The factorial of \b n, if <B>n>0</B> and
 * the factorial is able to be represented. \b 0 otherwise.
 *
 * \threadsafe_function_yes
 */
long long factorial(long long n) noexcept;

/**
 * \brief Calculate the factorial of the integer \b n, but let
 * us know if it overflowed.
 *
 * \param n a positive integer.
 * \param did_overflow whether the calculation overflowed, returned by reference.
 *
 * \return The factorial of \b n, if <B>n>0</B> and
 * the factorial is able to be represented. \b 0 otherwise.
 *
 * \threadsafe_function_ref
 */
long long factorial(long long n, bool &did_overflow) noexcept;
} // namespace math
} // namespace z
