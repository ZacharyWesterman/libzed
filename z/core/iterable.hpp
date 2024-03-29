#pragma once

namespace z
{
	namespace core
	{
		/**
		 * \interface iterable
		 * \brief A base interface for all objects that can be iterated over.
		*/
		template<typename ITER>
		class iterable
		{
		public:
			///Virtual destructor
			virtual ~iterable() noexcept {}

			/**
			* \brief Get an iterator for the first element in this object.
			*
			* This member function should not be used directly.
			* It is meant for C++11's range-based for loop syntax.
			*
			* \return An iterator on the first element.
			*/
			virtual ITER begin() const noexcept = 0;

			/**
			* \brief Get an iterator after the last element of this object.
			*
			* This member function should not be used directly.
			* It is meant for C++11's range-based for loop syntax.
			*
			* \return An iterator after the last element.
			*/
			virtual ITER end() const noexcept = 0;
		};
	}
}
