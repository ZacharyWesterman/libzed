#include "library.hpp"

#include <stdint.h>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <dlfcn.h>
#else
#warning file::library is incompatible with target OS! Please create a pull request or open an issue on GitHub.
#endif

namespace z {
namespace file {
library::library() noexcept {
	lib_ptr = 0;
}

library::~library() noexcept {
	unload();
}

bool library::load(const zpath &fileName, bool autoExtension) noexcept {
	auto fname = fileName;
#if defined(_WIN32)
	if (autoExtension) {
		fname.append(".dll");
	}
	if (lib_ptr) {
		FreeLibrary((HMODULE)lib_ptr);
	}
	lib_ptr = (void *)LoadLibrary((char *)fname.cstring());
	return (bool)lib_ptr;
#elif defined(__linux__)
	if (autoExtension) {
		fname.append(".so");
	}
	if (lib_ptr) {
		dlclose(lib_ptr);
	}
	lib_ptr = dlopen((char *)fname.cstring(), RTLD_NOW);
	return (bool)lib_ptr;
#else
	return false;
#endif
}

bool library::unload() noexcept {
#if defined(_WIN32)
	if (lib_ptr) {
		return !FreeLibrary((HMODULE)lib_ptr);
	} else {
		return true;
	}
#elif defined(__linux__)
	if (lib_ptr) {
		bool result = !dlclose(lib_ptr);
		lib_ptr = nullptr;
		return result;
	} else {
		return true;
	}
#else
	return true;
#endif
}

bool library::good() noexcept {
	return (bool)lib_ptr;
}

bool library::bad() noexcept {
	return !lib_ptr;
}

void *library::getRawSymbol(const zpath &symbol_name) noexcept {
	void *symbol_pointer = NULL;
#if defined(_WIN32)
	if (lib_ptr) {
		symbol_pointer = (void *)GetProcAddress((HMODULE)lib_ptr, (const char *)symbol_name.cstring());
	}
#elif defined(__linux__)
	if (lib_ptr) {
		symbol_pointer = dlsym(lib_ptr, (const char *)symbol_name.cstring());
	}
#endif

	return symbol_pointer;
}
} // namespace file
} // namespace z
