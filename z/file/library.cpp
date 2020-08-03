#include "library.hpp"

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
		library::library()
		{
			lib_ptr = 0;
		}

		library::~library()
		{
#			ifdef _WIN32
			if (lib_ptr) FreeLibrary((HMODULE)lib_ptr);
#			elif __linux__
			if (lib_ptr) dlclose(lib_ptr);
#			endif
		}

		bool library::load(const zpath& file_name)
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

		bool library::good()
		{
			return (bool)lib_ptr;
		}

		bool library::bad()
		{
			return !lib_ptr;
		}

		void* library::symbol(const zpath& symbol_name)
		{
			void* symbol_pointer = NULL;
#			ifdef _WIN32
			if (lib_ptr)
				symbol_pointer = (void*)GetProcAddress((HMODULE)lib_ptr, (char*)symbol_name.cstring());
#			elif __linux__
			if(lib_ptr)
				symbol_pointer = dlsym(lib_ptr, (const char*)symbol_name.cstring());
#			endif
			return symbol_pointer;
		}

		func library::function(const zpath& symbol_name)
		{
			void* symbol_pointer = NULL;
			func func_pointer = NULL;
#			ifdef _WIN32
			if (lib_ptr)
				symbol_pointer = (void*)GetProcAddress((HMODULE)lib_ptr, (const char*)symbol_name.cstring());
#			elif __linux__
			if(lib_ptr)
				symbol_pointer = dlsym(lib_ptr, (const char*)symbol_name.cstring());
#			endif
			if (symbol_pointer)
			{
				if (sizeof(void*) == 8) //64 bit pointers
					func_pointer = reinterpret_cast<func>(reinterpret_cast<long long>(symbol_pointer));
				else
					func_pointer = reinterpret_cast<func>(reinterpret_cast<long>(symbol_pointer));
			}
			return func_pointer;
		}

	}
}
