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
///   Date: 7/14/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_PLATFORM_OS_APPLE_MACH_KERN_H_
#define _XOS_PLATFORM_OS_APPLE_MACH_KERN_H_

#include "xos/platform/Os.h"

#if defined(APPLEOS)
#define kern_return_t platform_kern_return_t
#define KERN_SUCCESS PLATFORM_KERN_SUCCESS
#define KERN_FAILURE PLATFORM_KERN_FAILURE
#define KERN_ABORTED PLATFORM_KERN_ABORTED
#define KERN_OPERATION_TIMED_OUT PLATFORM_KERN_OPERATION_TIMED_OUT
#else /// defined(APPLEOS)
#endif /// defined(APPLEOS)

typedef int kern_return_t;
enum {
    KERN_SUCCESS             =  0,
    KERN_FAILURE             =  5,
    KERN_ABORTED             = 14,
    KERN_OPERATION_TIMED_OUT = 49,
};

#endif /*/ _XOS_PLATFORM_OS_APPLE_MACH_KERN_H_ /*/
