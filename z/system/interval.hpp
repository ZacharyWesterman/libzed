#pragma once

#include "../core/generator.hpp"

namespace z {
namespace system {

/**
 * @brief Create a generator that triggers at regular intervals.
 *
 * This generator will yield a dummy value every `millis` milliseconds.
 * The generator can will run indefinitely by default, but you can limit the number of triggers
 * by using the `limit` method on the generator.
 *
 * @param millis The interval in milliseconds between triggers.
 * @return A generator that yields every `millis` milliseconds.
 *
 * @warning This generator will block the thread for the specified interval,
 * so it should not be used where blocking is not acceptable.
 */
core::generator<bool, bool> interval(double millis) noexcept;

} // namespace system
} // namespace z
