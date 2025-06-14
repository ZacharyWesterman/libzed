#include "interval.hpp"
#include "sleep.hpp"

namespace z {
namespace system {

core::generator<bool, bool> interval(double millis) noexcept {
	return core::generator<bool, bool>(false, [millis](bool) {
		system::sleep(millis);
		return core::yield<bool>{false, false};
	});
}

} // namespace system
} // namespace z
