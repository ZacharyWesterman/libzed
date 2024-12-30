#pragma once

#include "generator.hpp"
#include "sentinel.hpp"

namespace z {
namespace core {

/**
 * @brief Generate a sequence of integers in a specified range.
 * @param begin The beginning of the range (inclusive).
 * @param end The end of the range (exclusive).
 * @param step The step size.
 * @return A generator that outputs a regular sequence of integers in the range.
 */
generator<long, long> range(long begin, long end, long step = 1) noexcept;

/**
 * @brief Generate a sequence of integers from 0 until the specified end point.
 * @param end The end of the range (exclusive).
 * @return A generator that outputs a regular sequence of integers in the range.
 */
generator<long, long> range(long end) noexcept;

/**
 * @brief Generate a sequence of integers in a specified range.
 * @param begin The beginning of the range (inclusive).
 * @param check The sentinel value that indicates when the end of the range has been reached.
 * @param step The step size.
 * @return A generator that outputs a regular sequence of integers in the range.
 */
generator<long, long> range(long begin, const sentinel &check, long step = 1) noexcept;

} // namespace core
} // namespace z