#pragma once

#if (__cplusplus >= 201703L) || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
// If C++17 or greater, use stdlib optional class.
#include <optional>
#elif !defined(DOXYGEN)
// Otherwise, polyfill our own implementation of std::optional

namespace std {

struct nullopt_t {
	explicit constexpr nullopt_t(int) noexcept {}
};
inline constexpr nullopt_t nullopt{0};

class bad_optional_access : public exception {
public:
	bad_optional_access() = default;
	virtual ~bad_optional_access() = default;

	const char *what() const noexcept override {
		return "bad optional access";
	}
};

template <typename T>
class optional {
	union {
		char dummy;
		T val;
	};
	bool engaged;

public:
	constexpr optional() noexcept : dummy(0), engaged(false) {}
	constexpr optional(nullopt_t) noexcept : dummy(0), engaged(false) {}

	optional(const optional &other) {
		engaged = other.engaged;
		if (engaged) {
			val = other.val;
		} else {
			dummy = 0;
		}
	};
	optional(optional &&other) noexcept(is_nothrow_move_constructible_v<T>) {
		engaged = other.engaged;
		if (engaged) {
			val = other.val;
		} else {
			dummy = 0;
		}
	}
	optional(T &&u) : val(std::forward<T>(u)), engaged(true) {}

	optional(const T &t) : val(t), engaged(true) {}

	optional &operator=(const optional &other) {
		engaged = other.engaged;
		if (engaged) {
			val = other.val;
		} else {
			dummy = 0;
		}
		return *this;
	}
	optional &operator=(const T &other) {
		if (engaged) {
			val.~T();
		}
		val.T(other);
		engaged = true;
		return *this;
	};
	optional &operator=(T &&other) {
		if (engaged) {
			val.~T();
		}
		val.T(other);
		engaged = true;
		return *this;
	}

	~optional() {
		if (engaged) {
			val.~T();
		}
	}

	constexpr operator bool() const {
		return engaged;
	}
	constexpr bool has_value() const {
		return engaged;
	}

	constexpr T &operator*() & {
		return val;
	}
	constexpr const T &operator*() const & {
		return val;
	}
	constexpr T &&operator*() && {
		return std::move(val);
	}
	constexpr const T &&operator*() const && {
		return std::move(val);
	}

	T &value() & {
		return engaged ? val : throw bad_optional_access();
	}
	const T &value() const & {
		return engaged ? val : throw bad_optional_access();
	}
	T &&value() && {
		return engaged ? std::move(val) : throw bad_optional_access();
	}
	const T &&value() const && {
		return engaged ? std::move(val) : throw bad_optional_access();
	}

	template <class U>
	T value_or(U &&u) const {
		return engaged ? val : static_cast<T>(std::forward<U>(u));
	}
};

} // namespace std

#endif
