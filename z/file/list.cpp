#include "list.h"

#ifdef __linux__
#include <dirent.h>
#elif _WIN32
#include <windows.h>
#else
#error file::list is incompatible with target OS.
#endif

namespace z
{
	namespace file
	{
		core::array< core::string<utf8> > listFiles(const core::string<utf8>& dir, const core::string<utf8>& file_type)
		{
			core::array< core::string<utf8> > output;

			core::string<utf8> search_path = dir;

			if (!dir.length())
				search_path += "./";
			else
				search_path += "/";


			#ifdef _WIN32
			search_path += "/*.";

			search_path += file_type;


			WIN32_FIND_DATA fd;
			HANDLE hFind = FindFirstFile((char*)search_path.cstring(), &fd);

			if (hFind != INVALID_HANDLE_VALUE)
			{
				do
				{
					if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						output.add(core::string<utf8>(fd.cFileName));
					}
				}
				while (FindNextFile(hFind, &fd));

				FindClose(hFind);
			}
			#else

			bool any_extension;

			any_extension = (file_type == "*");



			DIR *dpdf;
			dirent *epdf;

			dpdf = opendir((char*)search_path.cstring());

			if (dpdf)
			{
				while ((epdf = readdir(dpdf)))
				{
					core::string<utf8> filename (epdf->d_name);

					if (epdf->d_type != DT_DIR)
					{
						if (any_extension)
						{
							output.add(filename);
						}
						else
						{


							if (filename.endsWith(core::string<utf8>('.') + file_type))
								output.add(filename);
						}
					}
				}
			}

			closedir(dpdf);
			#endif

			return output;
		}


		core::array< core::string<utf8> > listDirs(const core::string<utf8>& dir, bool showAll)
		{
			core::array< core::string<utf8> > output;

			core::string<utf8> search_path = dir;

			if (!dir.length())
				search_path += "./";
			else
				search_path += "/";


			#ifdef _WIN32
			search_path += "/*";


			WIN32_FIND_DATA fd;
			HANDLE hFind = FindFirstFile((char*)search_path.cstring(), &fd);

			if (hFind != INVALID_HANDLE_VALUE)
			{
				do
				{
					if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						core::string<utf8> filename = fd.cFileName;

						if (showAll || (filename[0] != '.'))
							output.add(filename);
					}
				}
				while (FindNextFile(hFind, &fd));

				FindClose(hFind);
			}
			#else

			DIR *dpdf;
			dirent *epdf;

			dpdf = opendir((char*)search_path.cstring());

			if (dpdf)
			{
				while ((epdf = readdir(dpdf)))
				{
					core::string<utf8> filename (epdf->d_name);

					if (epdf->d_type == DT_DIR)
					{
						if (showAll || (filename[0] != '.'))
							output.add(filename);
					}
				}
			}

			closedir(dpdf);
			#endif

			return output;
		}
	}
}
