#include "timeout.h"

namespace z
{
	namespace core
	{
		timeout::timeout(size_t timeout_micros, bool run) : timer(run)
		{
			timeout_goal = timeout_micros;
		}

		void timeout::setTimeOut(size_t timeout_micros)
		{
			timeout_goal = timeout_micros;
		}

		bool timeout::timedOut() const
		{
			return ((timeout_goal >= 0) && (this->micros() >= timeout_goal));
		}
	}
}
