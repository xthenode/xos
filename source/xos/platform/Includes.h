/*/
///////////////////////////////////////////////////////////////////////
/// Copyright (c) 1988-2019 $organization$
///
/// This software is provided by the author and contributors ``as is'' 
/// and any express or implied warranties, including, but not limited to, 
/// the implied warranties of merchantability and fitness for a particular 
/// purpose are disclaimed. In no event shall the author or contributors 
/// be liable for any direct, indirect, incidental, special, exemplary, 
/// or consequential damages (including, but not limited to, procurement 
/// of substitute goods or services; loss of use, data, or profits; or 
/// business interruption) however caused and on any theory of liability, 
/// whether in contract, strict liability, or tort (including negligence 
/// or otherwise) arising in any way out of the use of this software, 
/// even if advised of the possibility of such damage.
///
///   File: Includes.h
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_PLATFORM_INCLUDES_H_
#define _XOS_PLATFORM_INCLUDES_H_

#include "xos/platform/Compiler.h"

#if defined(WINDOWS)
/*/
/// Windows
/// ...
/*/
#if !defined(_WINDOWS_)
#define _INC_WINDOWS
#include <windows.h>
#if !defined(_WIN32_WINNT)
#if (WINVER >= 0x0500)
#define _WIN32_WINNT WINVER
#endif /*/ (WINVER >= 0x500) /*/
#endif /*/ !defined(_WIN32_WINNT) /*/
#undef _INC_WINDOWS
#undef _WINDOWS_
#else /*/ !defined(_WINDOWS_) /*/
#endif /*/ !defined(_WINDOWS_) /*/

/*/
/// Winsock
/// ...
/*/
#if defined(WINSOCK_2)
/*/
/// Use Windows Sockets 2 API
/*/
#if defined(WINSOCK_DEFINED_DEBUG)
#error defined WINSOCK_2
#endif /*/ defined(WINSOCK_DEFINED_DEBUG) /*/
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(WINSOCK_1)
/*/
/// Use Windows Sockets 1 API
/*/
#if defined(WINSOCK_DEFINED_DEBUG)
#error defined WINSOCK_1
#endif /*/ defined(WINSOCK_DEFINED_DEBUG) /*/
#include <winsock.h>
#else /*/ defined(WINSOCK_2) /*/
/*/
/// Use Default Windows Sockets API
/// <windows.h>
/*/
#endif /*/ defined(WINSOCK_2) /*/
/*/
/// ...
/// Winsock
/*/

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <wchar.h>
#include <time.h>
#include <fcntl.h>
#include <io.h>
/*/
/// ...
/// Windows
/*/
#else /*/ defined(WINDOWS) /*/
/*/
/// Unix
/// ...
/*/
#if defined(MACOSX)
/*/
/// Mac OSX
/*/
#else /*/ defined(MACOSX) /*/
/*/
/// Otherwise
/*/
#endif /*/ defined(MACOSX) /*/

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
/*/
/// ...
/// Unix
/*/
#endif /*/ defined(WINDOWS) /*/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#endif /*/ _XOS_PLATFORM_INCLUDES_H_ /*/
