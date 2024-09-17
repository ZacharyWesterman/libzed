#include <iostream>
#include <z/all.hpp>

int main() {
	z::math::matrix<5> matrix;
	std::cout << matrix.det() << std::endl;
}