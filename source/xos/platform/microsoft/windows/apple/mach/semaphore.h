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
///   File: semaphore.h
///
/// Author: $author$
///   Date: 5/27/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_PLATFORM_MICROSOFT_WINDOWS_APPLE_MACH_SEMAPHORE_H_
#define _XOS_PLATFORM_MICROSOFT_WINDOWS_APPLE_MACH_SEMAPHORE_H_

#include "xos/platform/Os.h"
#include "xos/platform/microsoft/windows/apple/mach/task.h"

#if defined(__cplusplus)
extern "C" {
#endif /*/ defined(__cplusplus)  /*/

typedef mach_port_t semaphore_t;

kern_return_t semaphore_create (task_t task, semaphore_t *semaphore, int policy, int value);
kern_return_t semaphore_destroy (task_t task, semaphore_t semaphore);
kern_return_t semaphore_signal(semaphore_t semaphore);
kern_return_t semaphore_wait(semaphore_t semaphore);
kern_return_t semaphore_timedwait(semaphore_t semaphore, mach_timespec_t wait_time);

#if defined(__cplusplus)
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus)  /*/

#endif /*/ _XOS_PLATFORM_MICROSOFT_WINDOWS_APPLE_MACH_SEMAPHORE_H_ /*/
