#pragma once

#include <complex>

/**
 * \brief Greater-than comparison for complex values.
 *
 * \param arg1 the left-hand operand.
 * \param arg2 the right-hand operand.
 *
 * \return \b True if the normalization of \b arg1 is
 * greater than that of \b arg2. \b False otherwise.
 */
template <typename T>
const bool operator>(const std::complex<T>& arg1, const std::complex<T>& arg2)
{
	T norm1 = arg1.real()*arg1.real() + arg1.imag()*arg1.imag();
	T norm2 = arg2.real()*arg2.real() + arg2.imag()*arg2.imag();

	return norm1 > norm2;
}

/**
 * \brief Less-than comparison for complex values.
 *
 * \param arg1 the left-hand operand.
 * \param arg2 the right-hand operand.
 *
 * \return \b True if the normalization of \b arg1 is
 * less than that of \b arg2. \b False otherwise.
 */
template <typename T>
const bool operator<(const std::complex<T>& arg1, const std::complex<T>& arg2)
{
	T norm1 = arg1.real()*arg1.real() + arg1.imag()*arg1.imag();
	T norm2 = arg2.real()*arg2.real() + arg2.imag()*arg2.imag();

	return norm1 < norm2;
}

/**
 * \brief Greater-than comparison for complex values.
 *
 * \param arg1 the left-hand operand.
 * \param arg2 the right-hand operand.
 *
 * \return \b True if the normalization of \b arg1 is
 * greater than or equal to that of \b arg2. \b False
 * otherwise.
 */
template <typename T>
inline const bool operator>=(const std::complex<T>& arg1, const std::complex<T>& arg2)
{
	return !(operator<(arg1, arg2));
}

/**
 * \brief Less-than comparison for complex values.
 *
 * \param arg1 the left-hand operand.
 * \param arg2 the right-hand operand.
 *
 * \return \b True if the normalization of \b arg1 is
 * less than or equal to that of \b arg2. \b False
 * otherwise.
 */
template <typename T>
inline const bool operator<=(const std::complex<T>& arg1, const std::complex<T>& arg2)
{
	return !(operator>(arg1, arg2));
}
