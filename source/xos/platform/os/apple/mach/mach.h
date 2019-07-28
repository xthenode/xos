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
#define thread_t platform_thread_t
#define sync_policy_t platform_sync_policy_t
#define SYNC_POLICY_FIFO PLATFORM_SYNC_POLICY_FIFO
#define SYNC_POLICY_FIXED_PRIORITY PLATFORM_SYNC_POLICY_FIXED_PRIORITY
#define SYNC_POLICY_REVERSED PLATFORM_SYNC_POLICY_REVERSED
#define SYNC_POLICY_ORDER_MASK PLATFORM_SYNC_POLICY_ORDER_MASK
#define SYNC_POLICY_LIFO PLATFORM_SYNC_POLICY_LIFO
#else /// defined(APPLEOS)
#endif /// defined(APPLEOS)

typedef pointer_t mach_port_t;
typedef int clock_res_t;
typedef struct mach_timespec {
    unsigned int tv_sec;
    clock_res_t  tv_nsec;
} mach_timespec_t;

typedef mach_port_t thread_t;
typedef int sync_policy_t;
enum {
    SYNC_POLICY_FIFO            = 0x0,
    SYNC_POLICY_FIXED_PRIORITY  = 0x1,
    SYNC_POLICY_REVERSED        = 0x2,
    SYNC_POLICY_ORDER_MASK      = 0x3,
    SYNC_POLICY_LIFO            = (SYNC_POLICY_FIFO|SYNC_POLICY_REVERSED)
};

#endif /*/ _XOS_PLATFORM_OS_APPLE_MACH_MACH_H_ /*/
