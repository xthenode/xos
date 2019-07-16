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
///   File: mach.h
///
/// Author: $author$
///   Date: 7/14/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_PLATFORM_OS_APPLE_MACH_MACH_H_
#define _XOS_PLATFORM_OS_APPLE_MACH_MACH_H_

#include "xos/platform/os/apple/mach/kern.h"

#if defined(APPLEOS)
#define mach_port_t platform_mach_port_t
#define clock_res_t platform_clock_res_t
#define mach_timespec_t platform_mach_timespec_t
#define mach_timespec platform_mach_timespec
#else /// defined(APPLEOS)
#endif /// defined(APPLEOS)

typedef pointer_t mach_port_t;
typedef int clock_res_t;
typedef struct mach_timespec {
    unsigned int tv_sec;
    clock_res_t  tv_nsec;
} mach_timespec_t;

#endif /*/ _XOS_PLATFORM_OS_APPLE_MACH_MACH_H_ /*/
