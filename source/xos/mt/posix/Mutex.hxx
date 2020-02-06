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
///   File: Mutex.hxx
///
/// Author: $author$
///   Date: 6/21/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_POSIX_MUTEX_HXX_
#define _XOS_MT_POSIX_MUTEX_HXX_

#include "xos/mt/os/Os.hxx"
#include "xos/mt/Mutex.hxx"

#include <pthread.h>
#include <time.h>
#include <errno.h>

#if !defined(HAS_POSIX_TIMEOUTS)
#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#define HAS_POSIX_TIMEOUTS
#endif /// defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#endif /// !defined(HAS_POSIX_TIMEOUTS)

#if defined(HAS_POSIX_TIMEOUTS)
#if !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
#define PTHREAD_MUTEX_HAS_TIMEDLOCK
#endif /// !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
#endif /// defined(HAS_POSIX_TIMEOUTS)

#if !defined(CLOCK_HAS_GETTIME_RELATIVE_NP)
inline int clock_gettime_relative_np(struct timespec *res) {
    if ((res)) {
        res->tv_sec = 0;
        res->tv_nsec = 0;
        return 0; 
    }
    return EINVAL; 
}
#define CLOCK_HAS_GETTIME_RELATIVE_NP
#endif /// !defined(CLOCK_HAS_GETTIME_RELATIVE_NP)

#if !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
#if !defined(CLOCK_REALTIME)
#define CLOCK_REALTIME 0
#define clockid_t int
#endif /// !defined(CLOCK_REALTIME)
#if !defined(CLOCK_HAS_GETTIME)
inline int clock_gettime(clockid_t clk_id, struct timespec *res) {
    return ::clock_gettime_relative_np(res); 
}
#define CLOCK_HAS_GETTIME
#endif /// !defined(CLOCK_HAS_GETTIME)
inline int pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *abs_timeout) {
    return EINVAL; 
}
#define PTHREAD_MUTEX_HAS_TIMEDLOCK
#endif /// !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)

#if !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)
inline int pthread_mutex_timedlock_relative_np(pthread_mutex_t *mutex, const struct timespec *timeout) {
#if defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
    if ((mutex) && (timeout)) {
        int err = 0; struct timespec untilTime;
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &untilTime))) {
            untilTime.tv_sec +=  timeout->tv_sec;
            untilTime.tv_nsec +=  timeout->tv_nsec;
            err = ::pthread_mutex_timedlock(mutex, &untilTime);
        }
        return err;
    }
#endif /// defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
    return EINVAL;
}
#define PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP
#endif /// !defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)

namespace xos {
namespace mt {
namespace posix {

///////////////////////////////////////////////////////////////////////
///  Class: MutexT
///////////////////////////////////////////////////////////////////////
template 
<typename TError = int, TError VErrorSuccess = 0, TError VErrorFailed = -1,
 typename TAttached = pthread_mutex_t*, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::MutexT<TAttached, TUnattached, VUnattached>,
 class TMutex = extended::MutexT
 <TError, VErrorSuccess, VErrorFailed, TAttached, TUnattached, VUnattached, TImplement>,
 class TImplements = TImplement, class TExtends = TMutex>

class _EXPORTED_ MutexT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Extends::Attached Attached;
    static const typename Extends::UnattachedT Unattached = Extends::Unattached;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    MutexT(Attached detached, bool isCreated, bool isLogged, bool isErrLogged): Extends(detached, isCreated, isLogged, isErrLogged) {
    }
    MutexT(Attached detached, bool isCreated, bool isLogged): Extends(detached, isCreated, isLogged) {
    }
    MutexT(Attached detached, bool isCreated): Extends(detached, isCreated) {
    }
    MutexT(Attached detached): Extends(detached) {
    }
    MutexT(bool isLogged, bool isErrLogged): Extends(isLogged, isErrLogged) {
        XOS_MT_MUTEX_CREATED();
    }
    MutexT(bool isLogged): Extends(isLogged) {
        XOS_MT_MUTEX_CREATED();
    }
    MutexT() {
        XOS_MT_MUTEX_CREATED();
    }
    MutexT(const MutexT &copy): Extends(copy.AttachedTo(), false) {
    }
    virtual ~MutexT() {
        XOS_MT_MUTEX_DESTROYED();
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool Lock() { 
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->AttachedTo())) {
            return LockDetached(*detached);
        }
        return false; 
    }
    virtual LockStatus TryLock() { 
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->AttachedTo())) {
            return TryLockDetached(*detached);
        }
        return LockFailed; 
    }
    virtual LockStatus TimedLock(mseconds_t milliseconds) { 
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->AttachedTo())) {
            return TimedLockDetached(*detached, milliseconds);
        }
        return LockFailed; 
    }
    virtual LockStatus UntimedLock() { 
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->AttachedTo())) {
            return UntimedLockDetached(*detached);
        }
        return LockFailed; 
    }
    virtual bool Unlock() { 
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->AttachedTo())) {
            return UnlockDetached(*detached);
        }
        return false; 
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool LockDetached(pthread_mutex_t& mutex) const { 
        if (LockSuccess != (UntimedLockDetached(mutex))) {
            return false; 
        }
        return true; 
    }
    virtual LockStatus TimedLockDetached(pthread_mutex_t& mutex, mseconds_t milliseconds) const { 
        if (0 < (milliseconds)) {
#if defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
            bool isLogged = ((this->IsLogged()) && (milliseconds >= this->TimedLoggedThreasholdMilliseconds()));
            int err = 0;
            struct timespec untilTime;

            IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "clock_gettime(CLOCK_REALTIME, &untilTime)...");
            if ((err = clock_gettime(CLOCK_REALTIME, &untilTime))) {
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on clock_gettime(CLOCK_REALTIME, &untilTime)");
            } else {
                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...clock_gettime(CLOCK_REALTIME, &untilTime)");
            }
            untilTime.tv_sec +=  MSecondsSeconds(milliseconds);
            untilTime.tv_nsec +=  MSecondsNSeconds(MSecondsMSeconds(milliseconds));

            IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "pthread_mutex_timedlock(&mutex, &untilTime)...");
            if ((err = pthread_mutex_timedlock(&mutex, &untilTime))) {
                switch(err) {
                case EAGAIN:
                    IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...EAGAIN err = "<< err << " on pthread_mutex_timedlock(&mutex, &untilTime)");
                    return LockBusy;
                case EBUSY:
                    IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...EBUSY err = "<< err << " on pthread_mutex_timedlock(&mutex, &untilTime)");
                    return LockBusy;
                case ETIMEDOUT:
                    IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...ETIMEDOUT err = "<< err << " on pthread_mutex_timedlock(&mutex, &untilTime)");
                    return LockBusy;
                case EINTR:
                    IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on pthread_mutex_timedlock(&mutex, &untilTime)");
                    return LockInterrupted;
                default:
                    IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on pthread_mutex_timedlock(&mutex, &untilTime)");
                    return LockFailed;
                }
            } else {
                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...pthread_mutex_timedlock(&mutex, &untilTime)");
                return LockSuccess;
            }
#else /// defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
            IS_ERR_LOGGED_ERROR("...return LockInvalid");
            return LockInvalid;
#endif /// defined(PTHREAD_MUTEX_HAS_TIMEDLOCK)
        } else {
            return TryLockDetached(mutex);
        }
        return LockFailed; 
    }
    virtual LockStatus TryLockDetached(pthread_mutex_t& mutex) const { 
        int err = 0;
        IS_ERR_LOGGED_TRACE("::pthread_mutex_trylock(&mutex)...");
        if ((err = ::pthread_mutex_trylock(&mutex))) {
            switch(err) {
            case EAGAIN:
                IS_ERR_LOGGED_TRACE("...EAGAIN err = "<< err << " on ::pthread_mutex_trylock(&mutex)");
                return LockBusy;
            case EBUSY:
                IS_ERR_LOGGED_TRACE("...EBUSY err = "<< err << " on ::pthread_mutex_trylock(&mutex)");
                return LockBusy;
            case ETIMEDOUT:
                IS_ERR_LOGGED_TRACE("...ETIMEDOUT err = "<< err << " on ::pthread_mutex_trylock(&mutex)");
                return LockBusy;
            case EINTR:
                IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_mutex_trylock(&mutex)");
                return LockInterrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_mutex_trylock(&mutex)");
                return LockFailed;
            }
        } else {
            IS_ERR_LOGGED_TRACE("...::pthread_mutex_trylock(&mutex)");
            return LockSuccess;
        }
        return LockFailed; 
    }
    virtual LockStatus UntimedLockDetached(pthread_mutex_t& mutex) const { 
        int err = 0;
        IS_LOGGED_DEBUG("::pthread_mutex_lock(&mutex)...");
        if ((err = ::pthread_mutex_lock(&mutex))) {
            switch(err) {
            case EAGAIN:
                IS_ERR_LOGGED_ERROR("...EAGAIN err = "<< err << " on ::pthread_mutex_lock(&mutex)");
                return LockFailed;
            case EBUSY:
                IS_ERR_LOGGED_ERROR("...EBUSY err = "<< err << " on ::pthread_mutex_lock(&mutex)");
                return LockFailed;
            case ETIMEDOUT:
                IS_ERR_LOGGED_ERROR("...ETIMEDOUT err = "<< err << " on ::pthread_mutex_lock(&mutex)");
                return LockFailed;
            case EINTR:
                IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_mutex_lock(&mutex)");
                return LockInterrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_mutex_lock(&mutex)");
                return LockFailed;
            }
        } else {
            IS_LOGGED_DEBUG("...::pthread_mutex_lock(&mutex)");
            return LockSuccess;
        }
        return LockFailed; 
    }
    virtual bool UnlockDetached(pthread_mutex_t& mutex) const { 
        int err = 0;
        IS_LOGGED_DEBUG("::pthread_mutex_unlock(&mutex)...");
        if (!(err = ::pthread_mutex_unlock(&mutex))) {
            return true;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_mutex_unlock(&mutex)");
        }
        return false; 
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Attached CreateAttached() {
        Attached detached = ((Attached)Unattached);
        if ((detached = CreateDetached(_mutex))) {
            this->Attach(detached);
        }
        return detached;
    }
    virtual Attached CreateDetached(pthread_mutex_t& mutex) const {
        Attached detached = ((Attached)Unattached);
        int err = 0;
        pthread_mutexattr_t mutexattr;

        IS_ERR_LOGGED_DEBUG("::pthread_mutexattr_init(&mutexattr)...");
        if (!(err = ::pthread_mutexattr_init(&mutexattr))) {
            IS_ERR_LOGGED_DEBUG("::pthread_mutex_init(&mutex, &mutexattr)...");
            if (!(err = ::pthread_mutex_init(&mutex, &mutexattr))) {
                detached = &mutex;
            } else {
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_mutex_init(&mutex, &mutexattr)");
            }
            IS_ERR_LOGGED_DEBUG("::pthread_mutexattr_destroy(&mutexattr)...");
            if ((err = ::pthread_mutexattr_destroy(&mutexattr))) {
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_mutexattr_destroy(&mutexattr)");
                detached = ((Attached)Unattached);
                IS_ERR_LOGGED_DEBUG("::pthread_mutex_destroy(&mutex)...");
                if ((err = ::pthread_mutex_destroy(&mutex))) {
                    IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_mutex_destroy(&mutex)");
                }
            }
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_mutexattr_init(&mutexattr)");
        }
        return detached;
    }
    virtual bool DestroyDetached(Attached detached) const {
        if (((Attached)Unattached) != detached) {
            int err = 0;
            IS_ERR_LOGGED_DEBUG("::pthread_mutex_destroy(detached)...");
            if (((err = ::pthread_mutex_destroy(detached)))) {
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " ::pthread_mutex_destroy(detached)");
            } else {
                IS_ERR_LOGGED_DEBUG("...::pthread_mutex_destroy(detached)");
                return true;
            }
        }
        return false;
    }
    
protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    inline int clock_gettime(clockid_t clk_id, struct timespec *res) const {
#if defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)
        return ::clock_gettime_relative_np(res); 
#else /// defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)
        return ::clock_gettime(clk_id, res);
#endif /// defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)
    }
    inline int pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *abs_timeout) const {
#if defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)
        return ::pthread_mutex_timedlock_relative_np(mutex, abs_timeout);
#else /// defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)
        return ::pthread_mutex_timedlock(mutex, abs_timeout);
#endif /// defined(PTHREAD_MUTEX_HAS_TIMEDLOCK_RELATIVE_NP)
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    pthread_mutex_t _mutex;
}; /// class _EXPORTED_ MutexT
typedef MutexT<> Mutex;

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_POSIX_MUTEX_HXX_
