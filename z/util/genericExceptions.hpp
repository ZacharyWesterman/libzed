#include <exception>

namespace z
{
	namespace util
	{
		struct nonNumber : public std::exception
		{
			const char* what() const throw()
			{
				return "Attempted numeric operation on non-number";
			}
		};
	}
}
