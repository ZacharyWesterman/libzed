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

/**
 * @brief Template specialization for the skip function with generators that only output longs.
 * @param count The number of items to skip.
 * @return A new generator that skips the given number of items.
 */
template <>
generator<long, generator<long, long>::countedState> generator<long, long>::skip(long count);

} // namespace core
} // namespace z