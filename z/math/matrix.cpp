#include "matrix.hpp"

namespace z {
namespace math {
/**
 * \brief Calculate the determinant of a 2x2 matrix
 * \return The determinant.
 */
template <> double matrix<2>::det() noexcept {
	return (value[0][0] * value[1][1] - value[1][0] * value[0][1]);
}
} // namespace math
} // namespace z