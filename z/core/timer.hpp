#pragma once

#include <chrono>

namespace z
{
	namespace core
	{
		/**
		 * \brief A class for easily managing timers.
		 *
		 * Because this class uses the chrono library,
		 * it requires <B>at least</B> C++11.
		 */
		class timer
		{
		private:
			std::chrono::high_resolution_clock::time_point start;

		public:

			/**
			 * \brief Timer constructor.
			 *
			 * \param run an optional parameter to decide
			 * whether the timer should start now.
			 */
			timer (bool run = true);

			/**
			 * \brief Reset the timer.
			 */
			void reset();

			/**
			 * \brief Get the elapsed time in \b microseconds.
			 *
			 * \return The number of \b microseconds that have passed
			 * since the timer was last reset.
			 */
			unsigned int micros() const;

			/**
			 * \brief Get the elapsed time in \b milliseconds.
			 *
			 * \return The number of \b milliseconds that have passed
			 * since the timer was last reset.
			 */
			unsigned int millis() const;

			/**
			 * \brief Get the elapsed time in \b seconds.
			 *
			 * \return The number of \b seconds that have passed
			 * since the timer was last reset.
			 */
			unsigned int seconds() const;

			/**
			 * \brief Get the elapsed time in \b minutes.
			 *
			 * \return The number of \b minutes that have passed
			 * since the timer was last reset.
			 */
			unsigned int minutes() const;

			/**
			 * \brief Get the elapsed time in \b minutes.
			 *
			 * \return The number of \b minutes that have passed
			 * since the timer was last reset.
			 */
			unsigned int hours() const;
		};
	}
}
