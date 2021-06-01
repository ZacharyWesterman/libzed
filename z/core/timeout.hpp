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
			timeout(unsigned int timeout_micros = -1, bool run = true) noexcept;

			/**
			 * \brief Set the time out goal.
			 *
			 * \param timeout_micros the number of \b microseconds
			 * after which the timer will time out. If this is set
			 * to a negative value, then it will never time out.
			 */
			void setTimeOut(unsigned int timeout_micros) noexcept;

			/**
			 * \brief Check whether the timer has timed out.
			 *
			 * \return \b True if the timer has passed the specified
			 * time limit. \b False otherwise.
			 */
			bool timedOut() const noexcept;

#		ifdef __has_include
#		if __has_include(<cereal/cereal.hpp>)
			/**
			 * \brief Serialization output.
			 * \param ar The output archive.
			 */
			template <class Archive>
			void save(Archive& ar) const
			{
				time_t elapsed = micros();
				int goal = timeout_goal;
				ar(CEREAL_NVP(elapsed));
				ar(CEREAL_NVP(goal));
			}

			/**
			 * \brief Serialization input.
			 * \param ar The input archive.
			 */
			template <class Archive>
			void load(Archive& ar)
			{
				time_t elapsed;
				int goal;
				ar(CEREAL_NVP(elapsed));
				ar(CEREAL_NVP(goal));
				reset(std::chrono::high_resolution_clock::now() - std::chrono::microseconds(elapsed));
				timeout_goal = goal;
			}
#		endif
#		endif
		};
	}
}
