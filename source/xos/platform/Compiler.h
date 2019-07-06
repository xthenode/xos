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
///   File: Compiler.h
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_PLATFORM_COMPILER_H_
#define _XOS_PLATFORM_COMPILER_H_

#include "xos/platform/Build.h"

/*/
/// Determine compiler
/*/

#if defined(__MSC__) || defined(_MSC_VER) || defined(MSC_VER)
/*/
/// Microsoft C/C++
/// ...
/*/
#if defined(CPLATFORM_DEFINED_DEBUG)
#warning defined __MSC__
#endif /*/ defined(CPLATFORM_DEFINED_DEBUG) /*/

#if !defined(__MSC__)
#define __MSC__
#endif /*/ !defined(__MSC__) /*/

#if !defined(MSC_VER)
#define MSC_VER _MSC_VER
#endif /*/ !defined(MSC_VER) /*/

#define MSC_VER_6 1200
#define MSC_VER_7 1300
#define MSC_VER_8 1400
#define MSC_VER_9 1500
#define MSC_VER_10 1600
#define MSC_VER_11 1700
#define MSC_VER_12 1800
#define MSC_VER_14 1900

#if (_MSC_VER >= MSC_VER_14)
#define WINDOWS_MSC_VER_14
#elif (_MSC_VER >= MSC_VER_12)
#define WINDOWS_MSC_VER_12
#elif (_MSC_VER >= MSC_VER_11)
#define WINDOWS_MSC_VER_11
#elif (_MSC_VER >= MSC_VER_10)
#define WINDOWS_MSC_VER_10
#elif (_MSC_VER >= MSC_VER_9)
#define WINDOWS_MSC_VER_9
#elif (_MSC_VER >= MSC_VER_8)
#define WINDOWS_MSC_VER_8
#elif (_MSC_VER >= MSC_VER_7)
#define WINDOWS_MSC_VER_7
#else /*/ (_MSC_VER >= MSC_VER_14) /*/
#define WINDOWS_MSC_VER_6
#endif /*/ (_MSC_VER >= MSC_VER_14) /*/

/*/
/// ...
/// Microsoft C/C++
/*/
#endif /*/ defined(__MSC__) || defined(_MSC_VER) || defined(MSC_VER) /*/

#if defined(__GNUC__)
/*/
/// Gnu C/C++
/// ...
/*/
#if defined(CPLATFORM_DEFINED_DEBUG)
#warning defined __GNUC__
#endif /*/ defined(CPLATFORM_DEFINED_DEBUG) /*/

#define _T(string) string
/*/
/// ...
/// Gnu C/C++
/*/
#endif /*/ defined(__GNUC__) /*/

/*/
/// Determine language
/*/

#if defined(__OBJC__) || defined(__OBJC2__)
/*/
/// Objective C
/// ...
/*/
#if !defined(OBJC)
#define OBJC
#endif /*/ !defined(OBJC) /*/

#if defined(__OBJC2__)
#if !defined(OBJC2)
#define OBJC2
#endif /*/ !defined(OBJC2) /*/
#else /*/ defined(__OBJC2__) /*/
#if !defined(OBJC1)
#define OBJC1
#endif /*/ !defined(OBJC1) /*/
#endif /*/ defined(__OBJC2__) /*/
/*/
/// ...
/// Objective C
/*/
#endif /*/ defined(__OBJC__) || defined(__OBJC2__) /*/

#if defined(__cplusplus)
/*/
/// C++
/// ...
/*/
#if defined(__MSC__)
/*/
/// Microsoft C/C++
/// ...
/*/
/*/
/// Microsoft C/C++ version specific definitions
/// ...
/*/
#if (_MSC_VER >= MSC_VER_7)
/*/
/// Microsoft C/C++ version 7.0 or greater
/*/
#pragma warning(disable:4503) /*/ decorated name length exceeded, name was truncated /*/
#pragma warning(disable:4996) /*/ 'function' was declared deprecated /*/
#pragma warning(disable:4250) /*/ derived 'class' inherits 'base::member' via dominance /*/
#pragma warning(disable:4355) /*/ 'this' used in base member initializer list  /*/
#pragma warning(disable:4065) /*/ switch statement contains 'default' but no 'case' labels /*/
#else /*/ (_MSC_VER >= MSC_VER_7) /*/
/*/
/// Microsoft C/C++ version 6.0
/*/
#pragma warning(disable:4786) /*/ identifier was truncated to '255' /*/
                              /*/ characters in the debug information /*/
#define NO_TEMPLATE_PARAMETER_CAST
#define NO_STD_STRING_CLEAR
#endif /*/ (_MSC_VER >= MSC_VER_7) /*/

#if (_MSC_VER >= MSC_VER_12)
#if !defined(CPP_11)
#define CPP_11
#endif /*/ !defined(CPP_11) /*/
#endif /*/ (_MSC_VER >= MSC_VER_12) /*/
/*/
/// ...
/// Microsoft C/C++ version specific definitions
/*/

#define NO_ARRAY_CONSTRUCTION
#define NO_STATIC_MEMBER_EXPORT

#define PLATFORM_CCAST(type) const_cast<type>
#define PLATFORM_DCAST(type) dynamic_cast<type>
#define PLATFORM_SCAST(type) static_cast<type>
#define PLATFORM_RCAST(type) reinterpret_cast<type>

#define CTHIS
#define restrict
/*/
/// ...
/// Microsoft C/C++
/*/
#endif /*/ defined(__MSC__) /*/

#if defined(__GNUC__)
/*/
/// Gnu C/C++
/// ...
/*/
/*/
/// Gnu C/C++ version specific definitions
/// ...
/*/
#if (__GNUC__ < 4)
#define PLATFORM_RCAST(type) (type)
#else /*/ (__GNUC__ < 4) /*/
#define PLATFORM_RCAST(type) reinterpret_cast<type>
#if (__GNUC_MINOR__ < 3)
#define NO_ARRAY_CONSTRUCTION
#else /*/ (__GNUC_MINOR__ < 3) /*/
#endif /*/ (__GNUC_MINOR__ < 3) /*/
#endif /*/ (__GNUC__ < 4) /*/
/*/
/// ...
/// Gnu C/C++ version specific definitions
/*/
#if (__cplusplus >= 201100L)
#if !defined(CPP_11)
#define CPP_11
#endif /*/ !defined(CPP_11) /*/
#endif /*/ (__cplusplus >= 201100L) /*/

#define PLATFORM_CCAST(type) const_cast<type>
#define PLATFORM_DCAST(type) dynamic_cast<type>
#define PLATFORM_SCAST(type) static_cast<type>

#define NO_ARRAY_CONSTRUCTION
#define NO_TEMPLATE_STATIC_MEMBERS

#define CTHIS (*this).
/*/
/// ...
/// Gnu C/C++
/*/
#endif /*/ defined(__GNUC__) /*/

#if defined(OBJC)
/*/
/// Objective C++
/// ...
/*/
#if !defined(OBJECTIVE_CXX)
#define OBJECTIVE_CXX
#endif /*/ !defined(OBJECTIVE_CXX) /*/
/*/
/// ...
/// Objective C++
/*/
#endif /*/ defined(OBJC) /*/
/*/
/// ...
/// C++
/*/
#endif /*/ defined(__cplusplus)  /*/

#endif /*/ _XOS_PLATFORM_COMPILER_H_ /*/
