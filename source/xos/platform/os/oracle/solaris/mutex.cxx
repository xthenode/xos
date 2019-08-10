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
///   Date: 7/21/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/os/oracle/solaris/mutex.hxx"
#include "xos/mt/os/Mutex.hxx"

namespace xos {
namespace platform {
namespace os {
namespace oracle {
namespace solaris {

} /// namespace solaris
} /// namespace oracle
} /// namespace os
} /// namespace platform
} /// namespace xos

#if !defined(SOLARIS)
///
/// solaris mutexes
/// ...
int mutex_init(mutex_t *mp, int type, void *arg) {
    int err = EINVAL;
    if ((mp)) {
        ::xos::mt::os::Mutex* mtx = 0;
        if ((mtx = new ::xos::mt::os::Mutex(((::xos::mt::os::Mutex::Attached)::xos::mt::os::Mutex::Unattached), false, false))) {
            if ((mtx->Create())) {
                *mp = mtx;
                err = 0;
            } else {
                delete mtx;
            }
        }
    }
    return err;
}
int mutex_destroy(mutex_t *mp) {
    int err = EINVAL;
    if ((mp)) {
        ::xos::mt::os::Mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::Mutex*)(*mp)))) {
            if ((mtx->Destroy())) {
                err = 0;
            }
            *mp = 0;
            delete mtx;
        }
    }
    return err;
}
int mutex_lock(mutex_t *mp) {
    int err = EINVAL;
    if ((mp)) {
        ::xos::mt::os::Mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::Mutex*)(*mp)))) {
            if ((mtx->Lock())) {
                err = 0;
            }
        }
    }
    return err;
}
int mutex_trylock(mutex_t *mp) {
    int err = EINVAL;
    if ((mp)) {
        ::xos::mt::os::Mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::Mutex*)(*mp)))) {
            if (::xos::LockSuccess == (mtx->TryLock())) {
                err = 0;
            }
        }
    }
    return err;
}
int mutex_unlock(mutex_t *mp) {
    int err = EINVAL;
    if ((mp)) {
        ::xos::mt::os::Mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::Mutex*)(*mp)))) {
            if ((mtx->Unlock())) {
                err = 0;
            }
        }
    }
    return err;
}
int mutex_timedlock(mutex_t *mp, timestruc_t *abstime) {
    int err = EINVAL;
    if ((mp) && (abstime)) {
        ::xos::mt::os::Mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::Mutex*)(*mp)))) {
            timestruc_t reltime;
            if (!(err = ::clock_gettime(CLOCK_REALTIME, &reltime))) {
                reltime.tv_sec = abstime->tv_sec - reltime.tv_sec;
                reltime.tv_nsec = abstime->tv_nsec - reltime.tv_nsec;
                err = mutex_reltimedlock(mp, &reltime);
                return err;
            }
        }
    }
    return err;
}
int mutex_reltimedlock(mutex_t *mp, timestruc_t *reltime) {
    int err = EINVAL;
    if ((mp) && (reltime)) {
        ::xos::mt::os::Mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::Mutex*)(*mp)))) {
            mseconds_t milliseconds = ::xos::SecondsMSeconds(reltime->tv_sec) + ::xos::NSecondsMSeconds(reltime->tv_nsec);
            ::xos::LockStatus status = mtx->TimedLock(milliseconds);
            if (::xos::LockSuccess == (status)) {
                return 0;
            } else {
                if (::xos::LockBusy == (status)) {
                    return ETIME;
                } else {
                    if (::xos::LockInterrupted == (status)) {
                        return EINTR;
                    } else {
                    }
                }
            }
        }
    }
    return err;
}
/// ...
/// solaris mutexes
/// 
#endif /// !defined(SOLARIS)
