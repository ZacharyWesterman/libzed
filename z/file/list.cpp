#include "list.hpp"
#include <iostream>

namespace z {
namespace file {

core::generator<zpath, dirscan> listFiles(const zpath &dir, const zpath &fileType, bool showAll) noexcept {
	core::array<zpath> output;

	zpath searchPath = dir;

	if (!dir.length()) {
		searchPath += "./";
	} else {
		searchPath += "/";
	}

#if defined(_WIN32)
	searchPath += "/*.";
	searchPath += fileType;

	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(searchPath.cstring(), &fd);

	return core::generator<zpath, dirscan>(
		dirscan{
			fd,
			hFind,
			false,
			showAll,
		},
		[](dirscan &state) -> std::optional<zstring> {
			state.used = true;

			while (state.hFind != INVALID_HANDLE_VALUE) {
				if (state.fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					FindNextFile(state.hFind, &state.fd);
					continue;
				}

				if (!state.showAll && state.fd.cFileName[0] == '.') {
					continue;
				}

				const zpath filename(state.fd.cFileName);
				FindNextFile(state.hFind, &state.fd);

				return filename;
			}

			if (state.hFind != INVALID_HANDLE_VALUE) {
				FindClose(state.hFind);
				state.hFind = INVALID_HANDLE_VALUE;
			}
			return {};
		});
#else
	return core::generator<zpath, dirscan>(
		dirscan{
			opendir((char *)searchPath.cstring()),
			nullptr,
			fileType,
			false,
			showAll,
		},
		[](dirscan &state) -> std::optional<zstring> {
			state.used = true;

			// Get the next file
			while (state.dpdf && (state.epdf = readdir(state.dpdf))) {
				if (state.epdf->d_type == DT_DIR) {
					continue;
				}

				const zpath filename(state.epdf->d_name);

				if (state.fileType != '*' && !filename.endsWith('.'_zs + state.fileType)) {
					continue;
				}

				if (!state.showAll && filename[0] == '.') {
					continue;
				}

				return filename;
			}

			if (state.dpdf) {
				closedir(state.dpdf);
				state.dpdf = nullptr;
			}
			return {};
		});
#endif
}

core::generator<zpath, dirscan> listDirs(const zpath &dir, bool showAll) noexcept {
	core::array<zpath> output;

	zpath searchPath = dir;

	if (!dir.length()) {
		searchPath += "./";
	} else {
		searchPath += "/";
	}

#if defined(_WIN32)
	searchPath += "/*";

	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(searchPath.cstring(), &fd);

	return core::generator<zpath, dirscan>(
		dirscan{
			fd,
			hFind,
			false,
			showAll,
		},
		[](dirscan &state) -> std::optional<zstring> {
			state.used = true;

			while (state.hFind != INVALID_HANDLE_VALUE) {
				if (state.fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					FindNextFile(state.hFind, &state.fd);
					continue;
				}

				if (!state.showAll && state.fd.cFileName[0] == '.') {
					continue;
				}

				const zpath filename(state.fd.cFileName);
				FindNextFile(state.hFind, &state.fd);

				return filename;
			}

			if (state.hFind != INVALID_HANDLE_VALUE) {
				FindClose(state.hFind);
				state.hFind = INVALID_HANDLE_VALUE;
			}
			return {};
		});
#else
	return core::generator<zpath, dirscan>(
		dirscan{
			opendir((char *)searchPath.cstring()),
			nullptr,
			"",
			false,
			showAll,
		},
		[](dirscan &state) -> std::optional<zstring> {
			state.used = true;

			// Get the next file
			while (state.dpdf && (state.epdf = readdir(state.dpdf))) {
				if (state.epdf->d_type != DT_DIR) {
					continue;
				}

				const zpath filename(state.epdf->d_name);

				if (!state.showAll && filename[0] == '.') {
					continue;
				}

				return filename;
			}

			if (state.dpdf) {
				closedir(state.dpdf);
				state.dpdf = nullptr;
			}
			return {};
		});
#endif
}

} // namespace file
} // namespace z
