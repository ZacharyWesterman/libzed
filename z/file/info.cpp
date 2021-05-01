#include "info.hpp"

namespace z
{
	namespace file
	{
		info::info(const zpath& path) noexcept
		{
			does_exist = _stat((char*)path.cstring(), &fileStat);
		}

		bool info::exists() const noexcept
		{
			return does_exist;
		}

		time_t info::accessed() const noexcept
		{
			return does_exist ? fileStat.st_atime : 0;
		}

		time_t info::modified() const noexcept
		{
			return does_exist ? fileStat.st_mtime : 0;
		}

		time_t info::changed() const noexcept
		{
			return does_exist ? fileStat.st_ctime : 0;
		}

		size_t info::size() const noexcept
		{
			return does_exist ? fileStat.st_size : 0;
		}

		int info::device() const noexcept
		{
			return does_exist ? fileStat.st_dev : 0;
		}

		mode_t info::mode() const noexcept
		{
			return does_exist ? fileStat.st_mode : 0;
		}

		bool info::directory() const noexcept
		{
			return does_exist ? S_ISDIR(fileStat.st_mode) : false;
		}

		bool info::symlink() const noexcept
		{
			return does_exist ? S_ISLNK(fileStat.st_mode) : false;
		}

		bool info::regular() const noexcept
		{
			return does_exist ? S_ISREG(fileStat.st_mode) : false;
		}
	}
}
