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
///   File: condition.h
///
/// Author: $author$
///   Date: 7/29/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_PLATFORM_OS_ORACLE_SOLARIS_CONDITION_H_
#define _XOS_PLATFORM_OS_ORACLE_SOLARIS_CONDITION_H_

#include "xos/platform/os/oracle/solaris/mutex.h"

#if defined(__cplusplus)
extern "C" {
#endif /*/ defined(__cplusplus) /*/

typedef void* cond_t;

extern int cond_init(cond_t *cvp, int type, void *arg); 
extern int cond_destroy(cond_t *cvp);
extern int cond_wait(cond_t *cvp, mutex_t *mp);
extern int cond_timedwait(cond_t *cvp, mutex_t *mp, timestruc_t *abstime);
extern int cond_reltimedwait(cond_t *cvp, mutex_t *mp, timestruc_t *reltime);
extern int cond_signal(cond_t *cvp);
extern int cond_broadcast(cond_t *cvp);

#if defined(__cplusplus)
} /*/ extern "C" /*/
#endif /*/ defined(__cplusplus) /*/

#endif /*/ _XOS_PLATFORM_OS_ORACLE_SOLARIS_CONDITION_H_ /*/
