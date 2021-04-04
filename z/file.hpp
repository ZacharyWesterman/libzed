#pragma once
/**
 * \file z/file.hpp
 * \namespace z::file
 * \brief This section contains file management functions and classes.
**/

///Directory operations
#include "file/list.hpp"
#include "file/shorten.hpp"
#include "file/exists.hpp"
#include "file/makeDir.hpp"
#include "file/path.hpp"
#include "file/dir.hpp"
#include "file/chdir.hpp"
#include "file/size.hpp"

///Manage file contents
#include "file/copy.hpp"
#include "file/info.hpp"
#include "file/inputStream.hpp"
#include "file/outputStream.hpp"

///Dynamic library loading
#include "file/library.hpp"
