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
        class info
        {
        private:
            struct STAT fileStat;
            bool does_exist;

        public:
            info(const core::string<char>&);


            inline bool exists();

            time_t lastAccessed();
            time_t lastModified();
            time_t lastStatusChange();

            size_t byteCount();

            unsigned int deviceNumber();
        };


        info::info(const core::string<char>& path)
        {
            does_exist = STAT(path.str(), &fileStat);
        }



        inline bool info::exists()
        {
            return does_exist;
        }

        time_t info::lastAccessed()
        {
            if (does_exist)
                return fileStat.st_atime;
            else
                return 0;
        }

        time_t info::lastModified()
        {
            if (does_exist)
                return fileStat.st_mtime;
            else
                return 0;
        }

        time_t info::lastStatusChange()
        {
            if (does_exist)
                return fileStat.st_ctime;
            else
                return 0;
        }


        size_t info::byteCount()
        {
            if (does_exist)
                return fileStat.st_size;
            else
                return 0;
        }


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
