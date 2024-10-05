#include "progress.hpp"
#include "../system/terminal.hpp"
#include <ostream>

namespace z {
namespace util {

progress::progress(unsigned int update_freq_micros) noexcept : timer(update_freq_micros), displayed(false) {}

void progress::set(std::ostream &stream, long item, long max, const zstring &message, bool force) noexcept {
	if (force || !displayed || max == item || timer.timedOut()) {
		timer.reset();
		displayed = true;

		double percent = 100 * item / (double)max;

		auto pcnt = zstring::precision(percent, 1);
		zstring msg = message ? message + ' ' : "";

		msg = '\r'_zs + msg + pcnt + "% [";

		auto width = std::min(99, z::system::terminal().width - msg.length() - 1);
		auto rep = '#'_zs.repeat((percent / 100) * width);

		msg += rep + '-'_zs.repeat(width - rep.length()) + "] ";

		if (max == item) {
			msg.writeln(stream);
			displayed = false;
		} else {
			msg.write(stream);
		}
	}
}

void progress::done(std::ostream &stream) noexcept {
	if (displayed) {
		stream << std::endl;
	}
}

} // namespace util
} // namespace z