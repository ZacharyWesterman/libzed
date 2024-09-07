#pragma once

#include "../core/string.hpp"

namespace z
{
	namespace file
	{
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
			* \param autoExtension If true, automatically append the extension (".so" or ".dll" onto the file name).
			* Useful for loading libraries in a platform-independent way.
			*
			* \return \b True if the library loaded successfully.
			* \b False otherwise.
			*/
			bool load(const zpath& fileName, bool autoExtension = true) noexcept;

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
			* \warning It is up to the caller to indicate the correct
			* data type at compile time. Not doing so can cause crashes.
			*
			* \param symbolName the name of the symbol to retrieve.
			*
			* \return If a symbol with the given name was found, returns
			* a pointer to the symbol. Otherwise, if the symbol was not
			* found or the library hasn't been loaded, returns \b NULL.
			*/
			template<typename T>
			T* symbol(const zpath& symbolName) noexcept
			{
				return reinterpret_cast<T*>(getRawSymbol(symbolName));
			}

			/**
			* \brief Get a pointer to the function with the given name.
			*
			* This is similar to symbol(), except the symbol is assumed
			* to be a function.
			*
			* \warning It is up to the caller to indicate the correct
			* function signature at compile time. Not doing so can cause
			* crashes.
			*
			* Example usage:
			* To get a function of the form `bool func(int, float)`, call
			* `this_lib.function<bool, int, float>("symbol_name")`. To
			* get a function of the form `void func()`, call
			* `this_lib.function<void>("symbol_name")`.
			*
			* \param symbolName the name of the symbol to retrieve.
			*
			* \return If a symbol with the given name was found, returns
			* a pointer to the symbol. Otherwise, if the symbol was not
			* found or the library hasn't been loaded, returns \b NULL.
			*/
			template<typename RETURNTYPE, typename... PARAMTYPES>
			RETURNTYPE (* function(const zpath& symbolName))(PARAMTYPES...) noexcept
			{
				return reinterpret_cast<RETURNTYPE (*)(PARAMTYPES...)>(getRawSymbol(symbolName));
			}

		private:
			void* getRawSymbol(const zpath& symbolName) noexcept;
		};
	}
}
