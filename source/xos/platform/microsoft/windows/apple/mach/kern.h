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
///   File: kern.h
///
/// Author: $author$
///   Date: 5/27/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_PLATFORM_MICROSOFT_WINDOWS_APPLE_MACH_KERN_H_
#define _XOS_PLATFORM_MICROSOFT_WINDOWS_APPLE_MACH_KERN_H_

#include "xos/platform/Os.h"

#if defined(__cplusplus)
extern "C" {
#endif /*/ defined(__cplusplus)  /*/

typedef	int kern_return_t;
#define KERN_SUCCESS 0
#define KERN_INVALID_ARGUMENT 4
#define KERN_FAILURE 5
#define KERN_ABORTED 14
#define	KERN_OPERATION_TIMED_OUT 49

#if defined(__cplusplus)
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus)  /*/

#endif /*/ _XOS_PLATFORM_MICROSOFT_WINDOWS_APPLE_MACH_KERN_H_ /*/
