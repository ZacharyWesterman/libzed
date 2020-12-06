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
		library::library() noexcept
		{
			lib_ptr = 0;
		}

		library::~library() noexcept
		{
#			ifdef _WIN32
			if (lib_ptr) FreeLibrary((HMODULE)lib_ptr);
#			elif __linux__
			if (lib_ptr) dlclose(lib_ptr);
#			endif
		}

		bool library::load(const zpath& file_name) noexcept
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

		bool library::unload() noexcept
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

		bool library::good() noexcept
		{
			return (bool)lib_ptr;
		}

		bool library::bad() noexcept
		{
			return !lib_ptr;
		}

		void* library::symbol(const zpath& symbol_name) noexcept
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

		func library::function(const zpath& symbol_name) noexcept
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
#			if __x86_64__ || __ppc64__ || _WIN64
				func_pointer = reinterpret_cast<func>(reinterpret_cast<int64_t>(symbol_pointer));
#			else
				func_pointer = reinterpret_cast<func>(reinterpret_cast<int32_t>(symbol_pointer));
#			endif
			}
			return func_pointer;
		}

	}
}
