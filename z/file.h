/**
 * \file z/file.h
 * \namespace z::file
 * \brief Library of file management functions and classes.
**/

#pragma once
#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

///Directory operations
#include "file/list.h"
#include "file/shorten.h"
#include "file/exists.h"
#include "file/makeDir.h"

///Manage file contents
#include "file/reader.h"
#include "file/writer.h"
#include "file/copy.h"

///Dynamic library loading
#include "file/library.h"

#endif // FILE_H_INCLUDED
