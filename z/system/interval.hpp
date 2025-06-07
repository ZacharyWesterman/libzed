#pragma once

#include "../core/generator.hpp"

namespace z {
namespace system {

/**
 * @brief Create a generator that triggers at regular intervals.
 *
 * This generator will yield a dummy value every `millis` milliseconds.
 * The generator can be limited to a certain number of triggers using the `maxTriggers` parameter,
 * or by default, it will run indefinitely.
 *
 * @param millis The interval in milliseconds between triggers.
 * @param maxTriggers The maximum number of times the generator will yield before stopping. Use -1 for unlimited.
 * @return A generator that yields every `millis` milliseconds.
 *
 * @warning This generator will block the thread for the specified interval,
 * so it should not be used where blocking is not acceptable.
 */
core::generator<bool, long> interval(double millis, long maxTriggers = -1) noexcept;

} // namespace system
} // namespace z
