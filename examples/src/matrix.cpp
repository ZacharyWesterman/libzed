#include <z/all.hpp>
#include <iostream>

int main()
{
    z::math::matrix<5> matrix;
    std::cout << matrix.det() << std::endl;
}