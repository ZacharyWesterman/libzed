#include <iostream>
#include <type_traits>
#include <z/all.hpp>

template <typename T>
struct myclass {
	template <typename U = T, typename std::enable_if<z::core::has_iterator<U>::value, int>::type = 0>
	void foo() {}
};

int main() {
	myclass<int> bar;
	myclass<zstring> baz;
}
