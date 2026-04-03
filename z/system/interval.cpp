#include "interval.hpp"
#include "sleep.hpp"

namespace z {
namespace system {

core::generator<bool, bool> interval(double millis) noexcept {
	return core::generator<bool, bool>(false, [millis](bool) -> std::optional<bool> {
		system::sleep(millis);
		return false;
	});
}

} // namespace system
} // namespace z
