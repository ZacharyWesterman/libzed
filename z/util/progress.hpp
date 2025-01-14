#pragma once

#include "../core/string.hpp"
#include "../core/timeout.hpp"

namespace z {
namespace util {

/**
 * @brief A class for outputting a progress bar to the terminal.
 *
 * This class allows for easy progress bar displaying,
 * and automatically limits the frequency of stream writes for performance.
 */
class progress {
	core::timeout timer;
	bool displayed;

public:
	/**
	 * @brief Constructor.
	 * @param update_freq_micros The shortest time frame between stream writes. The stream will not update more frequently than this.
	 */
	progress(unsigned int update_freq_micros = 100000) noexcept;

	/**
	 * @brief Update the progress bar.
	 * @param stream The stream to write to.
	 * @param item The current item. Assumed to be in the range `0` to `max-1`, inclusive.
	 * @param max The total number of items. This will be 1 more than `item` can ever be.
	 * @param message An optional message to display in front of the percent.
	 * @param force If true, ignore frequency limit and force the stream to be updated.
	 */
	void set(std::ostream &stream, long item, long max, const zstring &message = "", bool force = false) noexcept;

	/**
	 * @brief Mark the progress bar as finished.
	 *
	 * If the progress bar has displayed any info, then this method appends a newline to the stream.
	 * If no info has been displayed, this method does nothing.
	 *
	 * @param stream The stream to write to.
	 */
	void done(std::ostream &stream) noexcept;
};

} // namespace util
} // namespace z