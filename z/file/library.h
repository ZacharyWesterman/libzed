#pragma once

#include <z/core/string.h>

namespace z
{
	namespace file
	{
		typedef void* lib;
		typedef void* ref;

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
			lib lib_ptr;

		public:
			/**
			 * \brief Default empty constructor.
			 */
			library();

			/**
			 * \brief Destructor frees any loaded library.
			 */
			~library();

			/**
			 * \brief Load a dynamic library with the given file name.
			 *
			 * \param file_name the path of the library to load.
			 *
			 * \return \b True if the library loaded successfully.
			 * \b False otherwise.
			 */
			bool load(const core::string<utf8>&);

			/**
			 * \brief Unload the dynamic library.
			 *
			 * \return \b False if unable to unload previously
			 * loaded library. \b True otherwise.
			 */
			bool unload();

			/**
			 * \brief Get whether the library has been loaded.
			 *
			 * \return \b True if the library has been loaded.
			 * \b False otherwise.
			 *
			 * \see bad()
			 */
			bool good();

			/**
			 * \brief Get whether the library has not been loaded.
			 *
			 * \return \b False if the library has been loaded.
			 * \b True otherwise.
			 *
			 * \see good()
			 */
			bool bad();

			/**
			 * \brief Get a pointer to the symbol with the given name.
			 *
			 * \param symbol_name the name of the symbol to retrieve.
			 *
			 * \return If a symbol with the given name was found, returns
			 * a pointer to the symbol. Otherwise, if the symbol was not
			 * found or the library hasn't been loaded, returns \b NULL.
			 */
			ref symbol(const core::string<utf8>&);
		};
	}
}
