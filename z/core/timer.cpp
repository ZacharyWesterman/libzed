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

		size_t timer::micros() const
		{
			return (size_t)std::chrono::duration_cast<std::chrono::microseconds>
					(std::chrono::high_resolution_clock::now() - start).count();
		}

		size_t timer::millis() const
		{
			return (micros() / 1000);
		}

		size_t timer::seconds() const
		{
			return (micros() / 1000000);
		}

		size_t timer::minutes() const
		{
			return (micros() / 60000000);
		}

		size_t timer::hours() const
		{
			return (micros() / 3600000000);
		}
	}
}
