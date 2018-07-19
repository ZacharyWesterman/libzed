#pragma once

namespace z
{
	namespace core
	{
		/**
		 * \interface sizable
		 * \brief An interface for getting an object's size.
		 */
		class sizable
		{
		public:
			///Virtual destructor
			virtual ~sizable() {}

			/**
			 * \brief Get the size of the object in memory
			 *
			 * \return The number of bytes in memory this object currently consumes.
			 */
			virtual size_t size() const = 0;
		};
	}
}
