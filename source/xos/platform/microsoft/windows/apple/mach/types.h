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
///   File: types.h
///
/// Author: $author$
///   Date: 5/27/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_PLATFORM_MICROSOFT_WINDOWS_APPLE_MACH_TYPES_H_
#define _XOS_PLATFORM_MICROSOFT_WINDOWS_APPLE_MACH_TYPES_H_

#include "xos/platform/Os.h"
#include "xos/platform/microsoft/windows/apple/mach/kern.h"

#if defined(__cplusplus)
extern "C" {
#endif /*/ defined(__cplusplus)  /*/

typedef pointer_t mach_port_t;

typedef int sync_policy_t;
#define SYNC_POLICY_FIFO		0x0
#define SYNC_POLICY_FIXED_PRIORITY	0x1
#define SYNC_POLICY_REVERSED		0x2
#define SYNC_POLICY_ORDER_MASK		0x3
#define SYNC_POLICY_LIFO		(SYNC_POLICY_FIFO|SYNC_POLICY_REVERSED)
#define SYNC_POLICY_MAX			0x7

typedef int	clock_res_t;
struct mach_timespec {
	unsigned int tv_sec;
	clock_res_t tv_nsec;
};
typedef struct mach_timespec mach_timespec_t;

#if defined(__cplusplus)
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus)  /*/

#endif /*/ _XOS_PLATFORM_MICROSOFT_WINDOWS_APPLE_MACH_TYPES_H_ /*/
