#pragma once

namespace z {
namespace system {
/**
 * @brief Delays program execution temporarily.
 *
 * This function is meant to be a
 * platform-independent way of allowing the user
 * to put the program to sleep for a little while.
 * Ideally, this could be used to keep the program
 * from taking up too much CPU time when it's not
 * doing anything important.
 *
 * @param ms the number of \b milliseconds to wait.
 *
 * @threadsafe_function_yes
 */
void pause(double ms) noexcept;
} // namespace system
} // namespace z
