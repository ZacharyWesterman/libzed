#include "library.h"

#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <dlfcn.h>
#else
#warning file::library is incompatible with target OS!
#endif

namespace z
{
	namespace file
	{
		/**
		 * \brief Default empty constructor.
		 */
		library::library()
		{
			lib_ptr = 0;
		}

		/**
		 * \brief Destructor frees any loaded library.
		 */
		library::~library()
		{
#			ifdef _WIN32
			if (lib_ptr) FreeLibrary((HMODULE)lib_ptr);
#			elif __linux__
			if (lib_ptr) dlclose(lib_ptr);
#			endif
		}

		/**
		 * \brief Load a dynamic library with the given file name.
		 *
		 * \param file_name the path of the library to load.
		 *
		 * \return \b True if the library loaded successfully.
		 * \b False otherwise.
		 */
		bool library::load(const core::string<utf8>& file_name)
		{
#			ifdef _WIN32
			if (lib_ptr) FreeLibrary((HMODULE)lib_ptr);
			lib_ptr = (void*)LoadLibrary((char*)file_name.cstring());
			return (bool)lib_ptr;
#			elif __linux__
			if (lib_ptr) dlclose(lib_ptr);
			lib_ptr = dlopen((char*)file_name.cstring(), RTLD_NOW);
			return (bool)lib_ptr;
#			else
			return false;
#			endif
		}

		/**
		 * \brief Unload the dynamic library.
		 *
		 * \return \b False if unable to unload previously
		 * loaded library. \b True otherwise.
		 */
		bool library::unload()
		{
#			ifdef _WIN32
			if (lib_ptr)
				return (bool)FreeLibrary((HMODULE)lib_ptr);
			else
				return true;
#			elif __linux__
			if (lib_ptr)
				return (bool)dlclose(lib_ptr);
			else
				return true;
#			else
			return true;
#			endif
		}

		/**
		 * \brief Get whether the library has been loaded.
		 *
		 * \return \b True if the library has been loaded.
		 * \b False otherwise.
		 *
		 * \see bad()
		 */
		bool library::good()
		{
			return (bool)lib_ptr;
		}

		/**
		 * \brief Get whether the library has not been loaded.
		 *
		 * \return \b False if the library has been loaded.
		 * \b True otherwise.
		 *
		 * \see good()
		 */
		bool library::bad()
		{
			return !lib_ptr;
		}

		/**
		 * \brief Get a pointer to the symbol with the given name.
		 *
		 * \param symbol_name the name of the symbol to retrieve.
		 *
		 * \return If a symbol with the given name was found, returns
		 * a pointer to the symbol. Otherwise, if the symbol was not
		 * found or the library hasn't been loaded, returns \b NULL.
		 */
		function library::symbol(const core::string<utf8>& symbol_name)
		{
#			ifdef _WIN32
			if (lib_ptr)
				return (function)GetProcAddress((HMODULE)lib_ptr, (char*)symbol_name.cstring());
			else
				return 0;
#			elif __linux__
			if(lib_ptr)
				return (function)dlsym(lib_ptr, (char*)symbol_name.cstring());
			else
				return 0;
#			else
			return 0;
#			endif
		}

	}
}
