#pragma once

#include <chrono>

#ifdef __has_include
#if __has_include(<cereal/cereal.hpp>)
#include <cereal/cereal.hpp>
#endif
#endif

namespace z {
namespace core {
/**
 * @brief A class for easily managing timers.
 *
 * Because this class uses the chrono library,
 * it requires <B>at least</B> C++11.
 */
class timer {
private:
	std::chrono::high_resolution_clock::time_point start;

public:
	/**
	 * @brief Timer constructor.
	 *
	 * @param run an optional parameter to decide
	 * whether the timer should start now.
	 */
	timer(bool run = true) noexcept;

	/**
	 * @brief Reset the timer.
	 */
	void reset() noexcept;

	/**
	 * @brief reset the timer to a specific point in time.
	 */
	void reset(std::chrono::high_resolution_clock::time_point time) noexcept;

	/**
	 * @brief Get the elapsed time in \b microseconds.
	 *
	 * @return The number of \b microseconds that have passed
	 * since the timer was last reset.
	 */
	unsigned int micros() const noexcept;

	/**
	 * @brief Get the elapsed time in \b milliseconds.
	 *
	 * @return The number of \b milliseconds that have passed
	 * since the timer was last reset.
	 */
	unsigned int millis() const noexcept;

	/**
	 * @brief Get the elapsed time in \b seconds.
	 *
	 * @return The number of \b seconds that have passed
	 * since the timer was last reset.
	 */
	unsigned int seconds() const noexcept;

	/**
	 * @brief Get the elapsed time in \b minutes.
	 *
	 * @return The number of \b minutes that have passed
	 * since the timer was last reset.
	 */
	unsigned int minutes() const noexcept;

	/**
	 * @brief Get the elapsed time in \b minutes.
	 *
	 * @return The number of \b minutes that have passed
	 * since the timer was last reset.
	 */
	unsigned int hours() const noexcept;

#ifdef __has_include
#if __has_include(<cereal/cereal.hpp>)
	/**
	 * @brief Serialization output.
	 * @param ar The output archive.
	 */
	template <class Archive>
	void save(Archive &ar) const {
		time_t elapsed = micros();
		ar(CEREAL_NVP(elapsed));
	}

	/**
	 * @brief Serialization input.
	 * @param ar The input archive.
	 */
	template <class Archive>
	void load(Archive &ar) {
		time_t elapsed;
		ar(CEREAL_NVP(elapsed));
		reset(std::chrono::high_resolution_clock::now() - std::chrono::microseconds(elapsed));
	}
#endif
#endif
};
} // namespace core
} // namespace z
