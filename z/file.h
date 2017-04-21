/**
 * File:            file.h
 * Namespace:       none
 * Description:     Header file which includes all
 *                  z::file methods.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   21 Apr. 2017
**/

#pragma once
#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

///Directory operations
#include "file/listFilesInDir.h"
#include "file/directoryOperations.h"

///Manage entire file contents
#include "file/loadFileToMemory.h"
#include "file/saveToFileFromMemory.h"

///Manage file contents with a timeout
#include "file/loadFileTimeout.h"

#endif // FILE_H_INCLUDED
