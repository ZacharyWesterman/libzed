/**
 * \file z/file.h
 * \namespace z::file
 * \brief Library of file management functions and classes.
**/

#pragma once
#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

///Directory operations
#include "file/listFilesInDir.h"
#include "file/directoryOperations.h"
#include "file/exists.h"

///Manage file contents
#include "file/reader.h"
#include "file/writer.h"

#endif // FILE_H_INCLUDED
