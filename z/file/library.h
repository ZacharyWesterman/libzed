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

        typedef HINSTANCE lib_t;
        typedef FARPROC smbl_t;

        #elif __linux__
        #include <dlfcn.h>

        ///Linux requires compiler flag -ldl

        typedef void* lib_t;
        typedef void* smbl_t;

        #else
        #warning "loadLibrary.h" is incompatible with target OS.

        typedef void* lib_t;
        typedef void* smbl_t;
        #endif

        class library
        {
        private:
            lib_t lib_ptr;

        public:
            library();
            ~library();

            bool load(const core::string<char>&);
            bool unload();

            inline bool good();
            inline bool bad();

            smbl_t symbol(const core::string<char>&);
        };


        library::library()
        {
            lib_ptr = NULL;
        }

        library::~library()
        {
            #ifdef _WIN32
            if (lib_ptr)
                FreeLibrary(lib_ptr);
            #elif __linux__
            if (lib_ptr)
                dlclose(lib_ptr);
            #endif
        }


        bool library::load(const core::string<char>& file_name)
        {
            #ifdef _WIN32
            lib_ptr = LoadLibrary(file_name.str());
            return (bool)lib_ptr;
            #elif __linux__
            lib_ptr = dlopen(file_name.str(), RTLD_NOW);
            return (bool)lib_ptr;
            #else
            return false;
            #endif
        }

        bool library::unload()
        {
            #ifdef _WIN32
            return (bool)FreeLibrary(lib_ptr);
            #elif __linux__
            return (bool)dlclose(lib_ptr);
            #else
            return false;
            #endif
        }


        inline bool library::good()
        {
            return (bool)lib_ptr;
        }

        inline bool library::bad()
        {
            return !lib_ptr;
        }


        smbl_t library::symbol(const core::string<char>& symbol_name)
        {
            #ifdef _WIN32
            if (lib_ptr)
                return GetProcAddress(lib_ptr, func_name.str());
            else
                return NULL;
            #elif __linux__
            if(lib_ptr)
                return dlsym(lib_ptr, symbol_name.str());
            else
                return NULL;
            #else
            return NULL;
            #endif
        }

    }
}

#endif // LOADLIBRARY_H_INCLUDED
