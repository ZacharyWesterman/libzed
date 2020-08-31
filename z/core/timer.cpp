#include "timer.hpp"

namespace z
{
	namespace core
	{
		timer::timer (bool run)
		{
			if (run) reset();
		}

		void timer::reset()
		{
			start = std::chrono::high_resolution_clock::now();
		}

		unsigned int timer::micros() const
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
		}

		unsigned int timer::millis() const
		{
			return (micros() / 1000);
		}

		unsigned int timer::seconds() const
		{
			return (micros() / 1000000);
		}

		unsigned int timer::minutes() const
		{
			return (micros() / 60000000);
		}

		unsigned int timer::hours() const
		{
			return (micros() / 3600000000);
		}
	}
}
