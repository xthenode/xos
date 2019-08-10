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
///   Date: 7/14/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_PLATFORM_OS_APPLE_MACH_SEMAPHORE_H_
#define _XOS_PLATFORM_OS_APPLE_MACH_SEMAPHORE_H_

#include "xos/platform/os/apple/mach/kern.h"
#include "xos/platform/os/apple/mach/mach.h"
#include "xos/platform/os/apple/mach/task.h"

#if defined(APPLEOS)
#define semaphore_t                platform_semaphore_t
#define semaphore_create           platform_semaphore_create
#define semaphore_destroy          platform_semaphore_destroy
#define semaphore_signal           platform_semaphore_signal
#define semaphore_signal_all       platform_semaphore_signal_all
#define semaphore_wait             platform_semaphore_wait
#define semaphore_trywait          platform_semaphore_trywait
#define semaphore_timedwait        platform_semaphore_timedwait
#define semaphore_timedwait_signal platform_semaphore_timedwait_signal
#define semaphore_wait_signal      platform_semaphore_wait_signal
#define semaphore_signal_thread    platform_semaphore_signal_thread
#endif /// defined(APPLEOS)

typedef pointer_t semaphore_t;

#if defined(__cplusplus)
extern "C" {
#endif /*/ defined(__cplusplus) /*/

extern kern_return_t semaphore_create(task_t task, semaphore_t *semaphore, int policy, int value);
extern kern_return_t semaphore_destroy(task_t task, semaphore_t semaphore);
extern kern_return_t semaphore_signal(semaphore_t semaphore);
extern kern_return_t semaphore_signal_all(semaphore_t semaphore);
extern kern_return_t semaphore_wait(semaphore_t semaphore);
extern kern_return_t semaphore_trywait(semaphore_t semaphore);
extern kern_return_t semaphore_timedwait(semaphore_t semaphore, mach_timespec_t wait_time);
extern kern_return_t semaphore_timedwait_signal(semaphore_t wait_semaphore, semaphore_t signal_semaphore, mach_timespec_t wait_time);
extern kern_return_t semaphore_wait_signal(semaphore_t wait_semaphore, semaphore_t signal_semaphore);
extern kern_return_t semaphore_signal_thread(semaphore_t semaphore, thread_t thread);

#if defined(__cplusplus)
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus) /*/

#endif /*/ _XOS_PLATFORM_OS_APPLE_MACH_SEMAPHORE_H_ /*/
