#include "range.hpp"

namespace z {
namespace core {

generator<long, long> range(long begin, long end, long step) noexcept {
	return generator<long, long>(begin, [end, step](long &state) {
		const long index = state;
		state += step;

		return yield<long>{
			(step >= 0) ? state > end : state < end,
			index,
		};
	});
}

generator<long, long> range(long end) noexcept {
	return range(0, end, 1);
}

} // namespace core
} // namespace z