#pragma once

#include "../core/string.hpp"

namespace z
{
	namespace file
	{
		///Typedef for library functions
		typedef void (* func)();

		/**
		 * \brief A class for loading dynamic libraries.
		 *
		 * This class is meant to allow an easy and
		 * platform-independent way to dynamically load libraries
		 * at run time. Note that if this is compiled for Linux,
		 * it is required that \b Z_DYNLIB is defined as well as
		 * including the linker flag <B>-ldl</B>.
		 */
		class library
		{
		private:
			void* lib_ptr;

		public:
			/**
			 * \brief Default empty constructor.
			 */
			library() noexcept;

			/**
			 * \brief Destructor frees any loaded library.
			 */
			~library() noexcept;

			/**
			 * \brief Load a dynamic library with the given file name.
			 *
			 * \param fileName the path of the library to load.
			 *
			 * \return \b True if the library loaded successfully.
			 * \b False otherwise.
			 */
			bool load(const zpath& fileName) noexcept;

			/**
			 * \brief Unload the dynamic library.
			 *
			 * \return \b False if unable to unload previously
			 * loaded library. \b True otherwise.
			 */
			bool unload() noexcept;

			/**
			 * \brief Get whether the library has been loaded.
			 *
			 * \return \b True if the library has been loaded.
			 * \b False otherwise.
			 *
			 * \see bad()
			 */
			bool good() noexcept;

			/**
			 * \brief Get whether the library has not been loaded.
			 *
			 * \return \b False if the library has been loaded.
			 * \b True otherwise.
			 *
			 * \see good()
			 */
			bool bad() noexcept;

			/**
			 * \brief Get a pointer to the symbol with the given name.
			 *
			 * \param symbolName the name of the symbol to retrieve.
			 *
			 * \return If a symbol with the given name was found, returns
			 * a pointer to the symbol. Otherwise, if the symbol was not
			 * found or the library hasn't been loaded, returns \b NULL.
			 */
			void* symbol(const zpath& symbolName) noexcept;

			/**
			 * \brief Get a pointer to the function with the given name.
			 *
			 * This is similar to symbol(), except the symbol is assumed
			 * to be a function. Note it is up to the program to cast to
			 * the correct function type.
			 *
			 * \param symbolName the name of the symbol to retrieve.
			 *
			 * \return If a symbol with the given name was found, returns
			 * a pointer to the symbol. Otherwise, if the symbol was not
			 * found or the library hasn't been loaded, returns \b NULL.
			 */
			func function(const zpath& symbolName) noexcept;
		};
	}
}
