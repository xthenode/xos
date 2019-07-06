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
///   File: Windows.h
///
/// Author: $author$
///   Date: 5/3/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_PLATFORM_MICROSOFT_WINDOWS_H_
#define _XOS_PLATFORM_MICROSOFT_WINDOWS_H_

#include "xos/platform/Configure.h"

#if defined(__cplusplus)
extern "C" {
#else /*/ defined(__cplusplus)  /*/
#endif /*/ defined(__cplusplus)  /*/

#if (_MSC_VER < MSC_VER_14)
#if !defined(va_copy)
#define va_copy(_target, _source) (_target = _source)
#endif // !defined(va_copy)
extern int vfscanf(FILE* file, const char* format, va_list va);
#endif // (_MSC_VER < MSC_VER_14)

#if defined(__cplusplus)
} /*/ extern "C" /*/
#else /*/ defined(__cplusplus)  /*/
#endif /*/ defined(__cplusplus)  /*/

#if defined(__cplusplus)
namespace xos {
namespace platform {
#endif /*/ defined(__cplusplus)  /*/

#if defined(__cplusplus)
#else /*/ defined(__cplusplus)  /*/
#endif /*/ defined(__cplusplus)  /*/

#if defined(__cplusplus)
} /*/ namespace platform /*/
} /*/ namespace xos /*/
#endif /*/ defined(__cplusplus)  /*/

#endif /*/ _XOS_PLATFORM_MICROSOFT_WINDOWS_H_ /*/
