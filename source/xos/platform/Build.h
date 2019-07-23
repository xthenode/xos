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
///   File: Build.h
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_PLATFORM_BUILD_H_
#define _XOS_PLATFORM_BUILD_H_

/*/
/// Determine build platform
/*/

/*/
/// Define WIN32 if any of the Win32 variants are defined
/*/
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__Win32__) || defined(__win32__)
#if defined(PLATFORM_DEFINED_DEBUG)
#error defined WIN32
#endif /*/ defined(PLATFORM_DEFINED_DEBUG) /*/
#if !defined(WIN32)
#define WIN32
#endif /*/ !defined(WIN32) /*/
#endif /*/ defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__Win32__) || defined(__win32__) /*/

/*/
/// Define WIN64 if any of the Win64 variants are defined
/*/
#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(__Win64__) || defined(__win64__)
#if defined(PLATFORM_DEFINED_DEBUG)
#error defined WIN64
#endif /*/ defined(PLATFORM_DEFINED_DEBUG) /*/
#if !defined(WIN64)
#define WIN64
#endif /*/ !defined(WIN64) /*/
#endif /*/ defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(__Win64__) || defined(__win64__) /*/

/*/
/// Define WINDOWS if any of the Windows variants are defined
/*/
#if defined(WIN64) || defined(WIN32)
#if defined(PLATFORM_DEFINED_DEBUG)
#error defined WINDOWS
#endif /*/ defined(PLATFORM_DEFINED_DEBUG) /*/
#if !defined(WINDOWS)
#define WINDOWS
#endif /*/ !defined(WINDOWS) /*/
#endif /*/ defined(WIN64) || defined(WIN32) /*/

/*/
/// Define APPLE if any of the Apple variants are defined
/*/
#if defined(APPLE) || defined(_APPLE) || defined(__APPLE__) || defined(__Apple__) || defined(__apple__)
#if defined(PLATFORM_DEFINED_DEBUG)
#error defined APPLE
#endif /*/ defined(PLATFORM_DEFINED_DEBUG) /*/
#if !defined(APPLE)
#define APPLE
#endif /*/ !defined(APPLE) /*/
#endif /*/ defined(APPLE) || defined(_APPLE) || defined(__APPLE__) || defined(__Apple__) || defined(__apple__) /*/

/*/
/// Define MACH if any of the Mach variants are defined
/*/
#if defined(MACH) || defined(_MACH) || defined(__MACH__) || defined(__Mach__) || defined(__mach__)
#if defined(PLATFORM_DEFINED_DEBUG)
#error defined MACH
#endif /*/ defined(PLATFORM_DEFINED_DEBUG) /*/
#if !defined(MACH)
#define MACH
#endif /*/ !defined(MACH) /*/
#endif /*/ defined(MACH) || defined(_MACH) || defined(__MACH__) || defined(__Mach__) || defined(__mach__) /*/

/*/
/// Define APPLEOS if Apple and Mach variants are defined
/*/
#if defined(APPLE) && defined(MACH)
#include "TargetConditionals.h"
#include "Availability.h"
#if defined(PLATFORM_DEFINED_DEBUG)
#error defined APPLEOS
#endif /*/ defined(PLATFORM_DEFINED_DEBUG) /*/
#if !defined(APPLEOS)
#define APPLEOS
#endif /*/ !defined(APPLEOS) /*/
#endif /*/ defined(APPLE) && defined(MACH) /*/

/*/
/// Define APPLEOSX if defined(APPLEOS) && (TARGET_OS_IPHONE == 0)
/*/
#if defined(APPLEOS) && (TARGET_OS_IPHONE == 0)
#if defined(PLATFORM_DEFINED_DEBUG)
#error defined APPLEOSX
#endif /*/ defined(PLATFORM_DEFINED_DEBUG) /*/
#if !defined(APPLEOSX)
#define APPLEOSX
#endif /*/ !defined(APPLEOSX) /*/
#endif /*/ defined(APPLEOS) && (TARGET_OS_IPHONE == 0) /*/

/*/
/// Define APPLEIOS if defined(APPLEOS) && (TARGET_OS_IPHONE != 0)
/*/
#if defined(APPLEOS) && (TARGET_OS_IPHONE != 0)
#if defined(PLATFORM_DEFINED_DEBUG)
#error defined APPLEIOS
#endif /*/ defined(PLATFORM_DEFINED_DEBUG) /*/
#if !defined(APPLEIOS)
#define APPLEIOS
#endif /*/ !defined(APPLEIOS) /*/
#endif /*/ defined(APPLEOS) && (TARGET_OS_IPHONE != 0) /*/

/*/
/// Define MACOSX if any of the AppleOSX variants are defined
/*/
#if defined(APPLEOSX) || defined(_APPLEOSX) || defined(__APPLEOSX__) || defined(__AppleOSX__) || defined(__appleosx__)
#if defined(PLATFORM_DEFINED_DEBUG)
#error defined MACOSX
#endif /*/ defined(PLATFORM_DEFINED_DEBUG) /*/
#if !defined(MACOSX)
#define MACOSX
#endif /*/ !defined(MACOSX) /*/
#endif /*/ defined(APPLEOSX) || defined(_APPLEOSX) || defined(__APPLEOSX__) || defined(__AppleOSX__) || defined(__appleosx__) /*/

/*/
/// Define LINUX if any of the Linux variants are defined
/*/
#if defined(LINUX) || defined(_LINUX) || defined(__LINUX__) || defined(__Linux__) || defined(__linux__)
#if defined(PLATFORM_DEFINED_DEBUG)
#error defined LINUX
#endif /*/ defined(PLATFORM_DEFINED_DEBUG) /*/
#if !defined(LINUX)
#define LINUX
#endif /*/ !defined(LINUX) /*/
#endif /*/ defined(LINUX) || defined(_LINUX) || defined(__LINUX__) || defined(__Linux__) || defined(__linux__) /*/

/*/
/// Define SUNOS if any of the SunOS variants are defined
/*/
#if defined(SUNOS) || defined(_SUNOS) || defined(__SUNOS__) || defined(__sun__) || defined(__sun) || defined(__SunOS)
#if defined(PLATFORM_DEFINED_DEBUG)
#error defined SUNOS
#endif /*/ defined(PLATFORM_DEFINED_DEBUG) /*/
#if !defined(SUNOS)
#define SUNOS
#endif /*/ !defined(SUNOS) /*/
#if !defined(SOLARIS)
#define SOLARIS
#endif /*/ !defined(SOLARIS) /*/
#endif /*/ defined(SUNOS) || defined(_SUNOS) || defined(__SUNOS__) || defined(__sun__) || defined(__sun) || defined(__SunOS) /*/

#if defined(__cplusplus)  
extern "C" {
#endif /*/ defined(__cplusplus)  /*/

#if defined(__cplusplus)  
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus)  /*/

#endif /*/ _XOS_PLATFORM_BUILD_H_ /*/
