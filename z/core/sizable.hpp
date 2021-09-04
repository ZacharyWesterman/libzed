#pragma once

#include <type_traits>
#include <cstdint>
#include <cstddef>

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
			virtual ~sizable() noexcept {}

			/**
			* \brief Get the size of the object in memory
			*
			* \return The number of bytes in memory this object currently consumes.
			*/
			virtual size_t size() const noexcept = 0;
		};

		/**
		* \brief Get the size of a sizable object.
		*
		* Calls the object's size() method.
		*
		* \return The number of bytes in memory this object currently consumes.
		*/
		template <typename T>
		inline typename std::enable_if<std::is_base_of<z::core::sizable, T>::value>::type
		size(const T& object, size_t& bytes) noexcept
		{
			bytes = object.size();
		}

		/**
		* \brief Get the size of a non-sizable object or datatype.
		*
		* \return The number of bytes in memory this object currently consumes.
		*/
		template <typename T>
		inline typename std::enable_if<!std::is_base_of<z::core::sizable, T>::value>::type
		size(const T& object, size_t& bytes) noexcept
		{
			bytes = sizeof(object);
		}
	}
}
