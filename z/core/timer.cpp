#include "timer.hpp"

namespace z {
namespace core {
timer::timer(bool run) noexcept {
	if (run)
		reset();
}

void timer::reset() noexcept {
	start = std::chrono::high_resolution_clock::now();
}

void timer::reset(std::chrono::high_resolution_clock::time_point time) noexcept {
	start = time;
}

unsigned int timer::micros() const noexcept {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
}

unsigned int timer::millis() const noexcept {
	return (micros() / 1000);
}

unsigned int timer::seconds() const noexcept {
	return (micros() / 1000000);
}

unsigned int timer::minutes() const noexcept {
	return (micros() / 60000000);
}

unsigned int timer::hours() const noexcept {
	return (micros() / 3600000000);
}
} // namespace core
} // namespace z
