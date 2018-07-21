/**
 * File:            lodaLibrary.h
 * Namespace:       z::core::library
 *
 * Description:     A platform-independent set of
 *                  functions for loading and unloading
 *                  libraries, and getting the functions
 *                  in those libraries.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Feb. 2017
**/

#pragma once
#ifndef LOADLIBRARY_H_INCLUDED
#define LOADLIBRARY_H_INCLUDED

#include <z/core/string.h>
#include <stdint.h>

#ifndef NULL
    #define NULL 0
#endif // NULL

namespace z
{
    namespace file
    {

        #ifdef _WIN32
        #include <windows.h>

        #define lib_t HINSTANCE
        typedef FARPROC smbl_t;

        #elif __linux__
        #include <dlfcn.h>

        #define lib_t void*
        typedef void* smbl_t;
        #else
        #warning "loadLibrary.h" is incompatible with target OS.

        #define lib_t void*

        ///Typedef for dynamically loaded symbol pointer (typically function pointer).
        typedef void* smbl_t;
        #endif

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
            lib_t lib_ptr;

        public:
            library();
            ~library();

            bool load(const core::string<utf8>&);
            bool unload();

            inline bool good();
            inline bool bad();

            smbl_t symbol(const core::string<utf8>&);
        };


        /**
         * \brief Default empty constructor.
         */
        library::library()
        {
            lib_ptr = NULL;
        }

        /**
         * \brief Destructor frees any loaded library.
         */
        library::~library()
        {
            #ifdef _WIN32
            if (lib_ptr)
                FreeLibrary(lib_ptr);
            #elif __linux__
            #ifdef Z_DYNLIB
            if (lib_ptr)
                dlclose(lib_ptr);
            #endif // Z_DYNLIB
            #endif
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
            #ifdef _WIN32
            if (lib_ptr)
                FreeLibrary(lib_ptr);
            lib_ptr = LoadLibrary(file_name.str());
            return (bool)lib_ptr;
            #elif __linux__
            #ifdef Z_DYNLIB
            if (lib_ptr)
                dlclose(lib_ptr);
            lib_ptr = dlopen(file_name.str(), RTLD_NOW);
            return (bool)lib_ptr;
            #else
            return false;
            #endif // Z_DYNLIB
            #else
            return false;
            #endif
        }

        /**
         * \brief Unload the dynamic library.
         *
         * \return \b False if unable to unload previously
         * loaded library. \b True otherwise.
         */
        bool library::unload()
        {
            #ifdef _WIN32
            if (lib_ptr)
                return (bool)FreeLibrary(lib_ptr);
            else
                return true;
            #elif __linux__
            #ifdef Z_DYNLIB
            if (lib_ptr)
                return (bool)dlclose(lib_ptr);
            else
                return true;
            #else
            return true;
            #endif // Z_DYNLIB
            #else
            return true;
            #endif
        }

        /**
         * \brief Get whether the library has been loaded.
         *
         * \return \b True if the library has been loaded.
         * \b False otherwise.
         *
         * \see bad()
         */
        inline bool library::good()
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
        inline bool library::bad()
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
        smbl_t library::symbol(const core::string<utf8>& symbol_name)
        {
            #ifdef _WIN32
            if (lib_ptr)
                return GetProcAddress(lib_ptr, symbol_name.str());
            else
                return NULL;
            #elif __linux__
            #ifdef Z_DYNLIB
            if(lib_ptr)
                return dlsym(lib_ptr, symbol_name.str());
            else
                return NULL;
            #else
            return NULL;
            #endif // Z_DYNLIB
            #else
            return NULL;
            #endif
        }

    }
}

#ifdef lib_t
    #undef lib_t
#endif // lib_t

#endif // LOADLIBRARY_H_INCLUDED
