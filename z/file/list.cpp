#include "list.hpp"

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
		core::array< zpath > listFiles(const zpath& dir, const zpath& file_type)
		{
			core::array< zpath > output;

			zpath search_path = dir;

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
						output.add(zpath(fd.cFileName));
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
					zpath filename (epdf->d_name);

					if (epdf->d_type != DT_DIR)
					{
						if (any_extension)
						{
							output.add(filename);
						}
						else
						{


							if (filename.endsWith(zpath('.') + file_type))
								output.add(filename);
						}
					}
				}
			}

			closedir(dpdf);
			#endif

			return output;
		}


		core::array< zpath > listDirs(const zpath& dir, bool showAll)
		{
			core::array< zpath > output;

			zpath search_path = dir;

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
						zpath filename = fd.cFileName;

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
					zpath filename (epdf->d_name);

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
