#pragma once
/**
 * @file z/file.hpp
 * @namespace z::file
 * @brief This section contains file management functions and classes.
 **/

/// Directory operations
#include "file/basename.hpp"
#include "file/chdir.hpp"
#include "file/dir.hpp"
#include "file/dirname.hpp"
#include "file/execdir.hpp"
#include "file/executable.hpp"
#include "file/exists.hpp"
#include "file/list.hpp"
#include "file/makeDir.hpp"
#include "file/path.hpp"
#include "file/shorten.hpp"
#include "file/size.hpp"

/// Manage file contents
#include "file/copy.hpp"
#include "file/info.hpp"
#include "file/lines.hpp"
#include "file/read.hpp"
#include "file/write.hpp"

/// Dynamic library loading
#include "file/library.hpp"
