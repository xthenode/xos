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
///   Date: 5/25/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_PLATFORM_MICROSOFT_WINDOWS_POSIX_SEMAPHORE_H_
#define _XOS_PLATFORM_MICROSOFT_WINDOWS_POSIX_SEMAPHORE_H_

#include "xos/platform/Os.h"
#include "xos/platform/microsoft/windows/posix/sys/time.h"

#if !defined(_POSIX_TIMEOUTS)
#define _POSIX_TIMEOUTS 1
#endif /*/ !defined(_POSIX_TIMEOUTS) /*/

#if defined(__cplusplus__)
extern "C" {
#endif /*/ defined(__cplusplus__) /*/

typedef pointer_t sem_t;

int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_destroy(sem_t *sem);

int sem_post(sem_t *sem);
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_timedwait(sem_t *sem, const struct timespec* timespec);

int sem_getvalue(sem_t *sem, int *sval);

#if defined(__cplusplus__)
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus__) /*/

#endif /*/ _XOS_PLATFORM_MICROSOFT_WINDOWS_POSIX_SEMAPHORE_H_ /*/
