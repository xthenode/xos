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
///   File: mutex.cxx
///
/// Author: $author$
///   Date: 6/21/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/posix/mutex.hxx"

#include <pthread.h>
#include <time.h>
#include <errno.h>

namespace xos {
namespace platform {
namespace posix {

} /// namespace posix
} /// namespace platform
} /// namespace xos

#if !defined(WINDOWS)
///
/// posix mutexes
/// ...
#if !defined(HAS_POSIX_TIMEOUTS)
#if defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
int pthread_mutex_timedlock
(pthread_mutex_t *mutex, const struct timespec *abs_timeout) {
    return EINVAL;
}
#endif /// defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)

#if defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)
int pthread_mutex_timedlock_relative_np
(pthread_mutex_t *mutex, const struct timespec *timeout) {
    if ((mutex) && (timeout)) {
        int err = 0;
        
        for (useconds_t uSecondsSleep = ::xos::MSecondsUSeconds(10),
             uSeconds = ::xos::SecondsUSeconds(timeout->tv_sec) + ::xos::NSecondsUSeconds(timeout->tv_nsec);
             uSecondsSleep <= uSeconds; uSeconds -= uSecondsSleep) {        

            if (EBUSY != (err = ::pthread_mutex_trylock(mutex))) {
                return err;
            }
            ::usleep(uSecondsSleep);
        }
        return ETIMEDOUT;
    }
    return EINVAL;
}
#endif /// defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)
#endif /// !defined(HAS_POSIX_TIMEOUTS)
/// ...
/// posix mutexes
/// 
#endif /// !defined(WINDOWS)
