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

		template <typename T>
		inline typename std::enable_if<std::is_base_of<z::core::sizable, T>::value>::type
		size(const T& object, size_t& bytes)
		{
			bytes = object.size();
		}

		template <typename T>
		inline typename std::enable_if<!std::is_base_of<z::core::sizable, T>::value>::type
		size(const T& object, size_t& bytes)
		{
			bytes = sizeof(object);
		}
	}
}
