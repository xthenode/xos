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
///   Date: 7/23/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_PLATFORM_OS_ORACLE_SOLARIS_SEMAPHORE_H_
#define _XOS_PLATFORM_OS_ORACLE_SOLARIS_SEMAPHORE_H_

#include "xos/platform/os/oracle/solaris/synch.h"

#if defined(SOLARIS)
#else /*/ defined(SOLARIS) /*/
#endif /*/ defined(SOLARIS) /*/

#if defined(__cplusplus)
extern "C" {
#endif /*/ defined(__cplusplus) /*/

typedef void* sema_t;

extern int sema_init(sema_t *sp, unsigned int count, int type, void *arg);
extern int sema_destroy(sema_t *sp);
extern int sema_post(sema_t *sp);
extern int sema_wait(sema_t *sp);
extern int sema_trywait(sema_t *sp);
extern int sema_timedwait(sema_t *sp, timestruc_t *abstime);
extern int sema_reltimedwait(sema_t *sp, timestruc_t *reltime);

#if defined(__cplusplus)
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus) /*/

#endif /*/ _XOS_PLATFORM_OS_ORACLE_SOLARIS_SEMAPHORE_H_ /*/
