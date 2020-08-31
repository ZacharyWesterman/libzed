#pragma once

#include "timer.hpp"

namespace z
{
	namespace core
	{
		/**
		 * \brief A class for easily managing timing.
		 *
		 * An extended version of the timer class
		 * which includes a timeout boundary.
		 */
		class timeout : public timer
		{
		private:
			unsigned int timeout_goal;

		public:
			/**
			 * \brief Timeout constructor.
			 *
			 * \param timeout_micros the number of \b microseconds
			 * after which the timer will time out. If this is set
			 * to a negative value, then it will never time out.
			 *
			 * \param run an optional parameter to decide
			 * whether the timer should start now.
			 */
			timeout(unsigned int timeout_micros = -1, bool run = true);

			/**
			 * \brief Set the time out goal.
			 *
			 * \param timeout_micros the number of \b microseconds
			 * after which the timer will time out. If this is set
			 * to a negative value, then it will never time out.
			 */
			void setTimeOut(unsigned int timeout_micros);

			/**
			 * \brief Check whether the timer has timed out.
			 *
			 * \return \b True if the timer has passed the specified
			 * time limit. \b False otherwise.
			 */
			bool timedOut() const;
		};
	}
}
