#include "list.hpp"
#include <iostream>

namespace z {
namespace file {

dirscan::~dirscan() {
	if (used) {
#ifdef __linux__
		if (dpdf) {
			closedir(dpdf);
		}
#elif _WIN32
		if (hFind != INVALID_HANDLE_VALUE) {
			FindClose(hFind);
		}
#endif
	}
}

core::generator<zpath, dirscan> listFiles(const zpath &dir, const zpath &fileType, bool showAll) noexcept {
	core::array<zpath> output;

	zpath searchPath = dir;

	if (!dir.length()) {
		searchPath += "./";
	} else {
		searchPath += "/";
	}

#ifdef _WIN32
	searchPath += "/*.";
	searchPath += fileType;

	return core::generator<zpath, dirscan>(
		dirscan{
			nullptr,
			nullptr,
			false,
			showAll,
		},
		[](dirscan &state) {
			state.used = true;

			if (!state.fd) {
				state.hFind = FindFirstFile((char *)search_path.cstring(), &state.fd);
			} else {
				FindNextFile(state.hFind, &state.fd);
			}

			while (state.hFind != INVALID_HANDLE_VALUE) {
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					FindNextFile(state.hFind, &state.fd);
					continue;
				}

				if (!state.showAll && fd.cFileName[0] == '.') {
					continue;
				}

				return core::yield<zpath>{false, fd.cFileName};
			}

			return core::yield<zpath>{true, ""};
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
		[](dirscan &state) {
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

				return core::yield<zpath>{false, filename};
			}

			return core::yield<zpath>{true, ""};
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

#ifdef _WIN32
	searchPath += "/*";
	searchPath += fileType;

	return core::generator<zpath, dirscan>(
		dirscan{
			nullptr,
			nullptr,
			false,
			showAll,
		},
		[](dirscan &state) {
			state.used = true;

			if (!state.fd) {
				state.hFind = FindFirstFile((char *)search_path.cstring(), &state.fd);
			} else {
				FindNextFile(state.hFind, &state.fd);
			}

			while (state.hFind != INVALID_HANDLE_VALUE) {
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					FindNextFile(state.hFind, &state.fd);
					continue;
				}

				if (!state.showAll && fd.cFileName[0] == '.') {
					continue;
				}

				return core::yield<zpath>{false, fd.cFileName};
			}

			return core::yield<zpath>{true, ""};
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
		[](dirscan &state) {
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

				return core::yield<zpath>{false, filename};
			}

			return core::yield<zpath>{true, ""};
		});
#endif
}

} // namespace file
} // namespace z
