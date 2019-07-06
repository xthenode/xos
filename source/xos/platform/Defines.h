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
///   File: Defines.h
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_PLATFORM_DEFINES_H_
#define _XOS_PLATFORM_DEFINES_H_

#include "xos/platform/Includes.h"

#if defined(WINDOWS)
/*/
// Windows
// ...
/*/
#if !defined(__GNUC__)
/*/
// Microsoft C/C++
// ...
/*/
/*/
// ...
// Microsoft C/C++
/*/
#else /*/ !defined(__GNUC__) /*/
/*/
// Gnu C/C++
// ...
/*/
#if defined(CPLATFORM_DEFINED_DEBUG)
#error Compiler other than Microsoft C/C++ (Gnu C/C++) being used
#endif /*/ defined(CPLATFORM_DEFINED_DEBUG) /*/
/*/
// ...
// Gnu C/C++
/*/
#endif /*/ !defined(__GNUC__) /*/

#if defined(UNICODE)
#define TCHAR_NOT_CHAR
#endif /*/ defined(UNICODE) /*/

#define PLATFORM_EXPORT __declspec(dllexport)
#define PLATFORM_IMPORT __declspec(dllimport)

#define FOREIGN_DIRECTORY_SEPARATOR_CHAR '/'
#define PLATFORM_DIRECTORY_SEPARATOR_CHAR '\\'
#define PLATFORM_VOLUME_SEPARATOR_CHAR ':'
/*/
// ...
// Windows
/*/
#else /*/ defined(WINDOWS) /*/
/*/
// Unix
// ...
/*/
#if defined(__GNUC__)
/*/
// Gnu C/C++
// ...
/*/
/*/
// ...
// Gnu C/C++
/*/
#else /*/ defined(__GNUC__) /*/
#error Compiler other than Gnu C/C++ not supported
#endif /*/ defined(__GNUC__) /*/

#define PLATFORM_EXPORT
#define PLATFORM_IMPORT

#if defined(__stdcall)
#define PLATFORM___stdcall __stdcall
#undef __stdcall
#endif /*/ defined(__stdcall) /*/
#define __stdcall

#if defined(unix)
#define PLATFORM_unix unix
#undef unix
#endif /*/ defined(unix) /*/

#if defined(MACOSX)
/*/
// MacOSX
// ...
/*/
#if !defined(PLATFORM_HAS_NO_EXECVPE)
#define PLATFORM_HAS_NO_EXECVPE
#endif /*/ !defined(PLATFORM_HAS_NO_EXECVPE) /*/
/*/
// ...
// MacOSX
/*/
#endif /*/ defined(MACOSX) /*/

#define FOREIGN_DIRECTORY_SEPARATOR_CHAR '\\'
#define PLATFORM_DIRECTORY_SEPARATOR_CHAR '/'
#define PLATFORM_VOLUME_SEPARATOR_CHAR 0

#define TRUE 1
#define FALSE 0

#define INFINITE -1

#define WINAPI
#define FAR
#define _In_
#define _Out_
#define _Inout_
#define _In_opt_
#define _Out_opt_
#define _Inout_opt_
/*/
// ...
// Unix
/*/
#endif /*/ defined(WINDOWS) /*/

#if defined(__MSC__)
/*/
// Microsoft C/C++
// ...
/*/
#define PLATFORM_PACKED
/*/
// ...
// Microsoft C/C++
/*/
#else /*/ defined(__MSC__) /*/
#if defined(__GNUC__)
/*/
// Gnu C/C++
// ...
/*/
#define PLATFORM_PACKED __attribute__ ((packed))
/*/
// ...
// Gnu C/C++
/*/
#else /*/ defined(__GNUC__) /*/
#error Compiler not supported
#endif /*/ defined(__GNUC__) /*/
#endif /*/ defined(__MSC__) /*/

/*/
// Generic
// ...
/*/
#define UNDEFINED -1

#define PLATFORM_EXTENSION_SEPARATOR "."
#define FOREIGN_EXTENSION_SEPARATOR PLATFORM_EXTENSION_SEPARATOR

#define PLATFORM_EXTENSION_SEPARATOR_CHAR '.'
#define FOREIGN_EXTENSION_SEPARATOR_CHAR PLATFORM_EXTENSION_SEPARATOR_CHAR

#define _PLATFORM_IMPORT PLATFORM_IMPORT
#define _PLATFORM_EXPORT PLATFORM_EXPORT

#define c_EXPORT PLATFORM_EXPORT
#define c_IMPORT PLATFORM_IMPORT

#define PACKED_STRUCT PLATFORM_PACKED
#define _PACKED_STRUCT PLATFORM_PACKED
#define c_PACKED_STRUCT PLATFORM_PACKED

#if defined(DELETE)
#define PLATFORM_DELETE DELETE
#undef DELETE
#endif /*/ defined(DELETE) /*/
/*/
// ...
// Generic
/*/

#if defined(__cplusplus)
/*/
/// C++
/// ...
/*/
#define EXPORT_FUNCTION PLATFORM_EXPORT
#define EXPORT_CLASS PLATFORM_EXPORT
#define INTERFACE_CLASS PLATFORM_EXPORT
#define IMPLEMENT_CLASS PLATFORM_EXPORT
#define INSTANCE_CLASS PLATFORM_EXPORT

#define _EXPORT_FUNCTION PLATFORM_EXPORT
#define _EXPORT_CLASS PLATFORM_EXPORT
#define _INTERFACE_CLASS PLATFORM_EXPORT
#define _IMPLEMENT_CLASS PLATFORM_EXPORT
#define _INSTANCE_CLASS PLATFORM_EXPORT

#define c_EXPORT_FUNCTION PLATFORM_EXPORT
#define c_EXPORT_CLASS PLATFORM_EXPORT
#define c_INTERFACE_CLASS PLATFORM_EXPORT
#define c_IMPLEMENT_CLASS PLATFORM_EXPORT
#define c_INSTANCE_CLASS PLATFORM_EXPORT
/*/
/// ...
/// C++
/*/
#endif /*/ defined(__cplusplus)  /*/

#endif /*/ _XOS_PLATFORM_DEFINES_H_ /*/
