#include "interval.hpp"
#include "sleep.hpp"

namespace z {
namespace system {

core::generator<bool, long> interval(double millis, long maxTriggers) noexcept {
	return core::generator<bool, long>(false, [millis, maxTriggers](long &triggers) {
		if (maxTriggers >= 0 && triggers++ >= maxTriggers) {
			return core::yield<bool>{true, false};
		}

		system::sleep(millis);
		return core::yield<bool>{false, false};
	});
}

} // namespace system
} // namespace z
