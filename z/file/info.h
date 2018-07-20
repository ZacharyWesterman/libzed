#pragma once
#ifndef INFO_H_INCLUDED
#define INFO_H_INCLUDED

#include <sys/stat.h>
#include <z/core/string.h>

#ifdef _WIN32
#define STAT _stat
#else
#define STAT stat
#endif

namespace z
{
    namespace file
    {
        /**
         * \brief A class to allow a platform-independent
         * way to get information on a file.
         *
         * As not all Linux file system options have a
         * Windows analog, this class is not a full wrapper
         * for the \b stat struct. It does however allow for
         * retrieval of the most common file information
         * (e.g. file size, modification date, etc).
         */
        class info
        {
        private:
            struct STAT fileStat;
            bool does_exist;

        public:
            info(const core::string<utf8>&);


            inline bool exists();

            time_t lastAccessed();
            time_t lastModified();
            time_t lastStatusChange();

            size_t byteCount();

            unsigned int deviceNumber();
        };


        /**
         * \brief Constructor with file path.
         *
         * \param path a string of the path to
         * the file object.
         */
        info::info(const core::string<utf8>& path)
        {
            does_exist = STAT(path.cstring(), &fileStat);
        }

        /**
         * \brief Get whether the file object exists
         * and can be accessed.
         *
         * \return \b False if the file does not exist
         * or cannot be accessed for some reason.
         * \b True otherwise.
         */
        inline bool info::exists()
        {
            return does_exist;
        }

        /**
         * \brief Get the time that the file was last accessed.
         *
         * \return When the file was last accessed, if it exists.
         * \b 0 otherwise.
         */
        time_t info::lastAccessed()
        {
            if (does_exist)
                return fileStat.st_atime;
            else
                return 0;
        }

        /**
         * \brief Get the time that the file was last modified.
         *
         * \return When the file was last modified, if it exists.
         * \b 0 otherwise.
         */
        time_t info::lastModified()
        {
            if (does_exist)
                return fileStat.st_mtime;
            else
                return 0;
        }

        /**
         * \brief Get the time that the file's status was last changed.
         *
         * \return When the file status was last changed, if it exists.
         * \b 0 otherwise.
         */
        time_t info::lastStatusChange()
        {
            if (does_exist)
                return fileStat.st_ctime;
            else
                return 0;
        }

        /**
         * \brief Get the size of the file.
         *
         * \return The size of the file (in Bytes), if it exists.
         * \b 0 otherwise.
         */
        size_t info::byteCount()
        {
            if (does_exist)
                return fileStat.st_size;
            else
                return 0;
        }

        /**
         * \brief Get the number of the device where the file is stored.
         *
         * \return The device number of the file, if it exists.
         * \b 0 otherwise.
         */
        unsigned int info::deviceNumber()
        {
            if (does_exist)
                return fileStat.st_dev;
            else
                return 0;
        }
    }
}


#undef STAT

#endif // INFO_H_INCLUDED
