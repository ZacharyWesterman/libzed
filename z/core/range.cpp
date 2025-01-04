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

generator<long, long> range(long begin, const sentinel &check, long step) noexcept {
	return generator<long, long>(begin, [&check, step](long &state) {
		const long index = state;
		state += step;

		return yield<long>{
			check(state, step),
			index,
		};
	});
}

template <>
generator<long, generator<long, long>::countedState> generator<long, long>::skip(long count) {
	auto lambda = this->lambda;

	return generator<long, generator<long, long>::countedState>({count, state}, [lambda](countedState &state) {
		if (state.count > 0) {
			state.state += state.count;
		}
		state.count = 0;

		return lambda(state.state);
	});
}

} // namespace core
} // namespace z