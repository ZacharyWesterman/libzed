#include "timeout.hpp"

namespace z
{
	namespace core
	{
		timeout::timeout(unsigned int timeout_micros, bool run) : timer(run)
		{
			timeout_goal = timeout_micros;
		}

		void timeout::setTimeOut(unsigned int timeout_micros)
		{
			timeout_goal = timeout_micros;
		}

		bool timeout::timedOut() const
		{
			return this->micros() >= timeout_goal;
		}
	}
}
