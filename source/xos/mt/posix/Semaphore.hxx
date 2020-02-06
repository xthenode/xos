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
///   File: Semaphore.hxx
///
/// Author: $author$
///   Date: 6/23/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_POSIX_SEMAPHORE_HXX_
#define _XOS_MT_POSIX_SEMAPHORE_HXX_

#include "xos/mt/os/Os.hxx"
#include "xos/mt/Semaphore.hxx"

#include <semaphore.h>
#include <errno.h>

#if !defined(HAS_POSIX_TIMEOUTS)
#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#define HAS_POSIX_TIMEOUTS
#endif /// defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#endif /// !defined(HAS_POSIX_TIMEOUTS)

#if defined(HAS_POSIX_TIMEOUTS)
#if !defined(POSIX_SEM_HAS_TIMEDWAIT)
#define POSIX_SEM_HAS_TIMEDWAIT
#endif /// !defined(POSIX_SEM_HAS_TIMEDWAIT)
#endif /// defined(HAS_POSIX_TIMEOUTS)

#if !defined(POSIX_SEM_HAS_TIMEDWAIT)
#if defined(SEM_HAS_TIMEDWAIT)
#define POSIX_SEM_HAS_TIMEDWAIT
#endif /// defined(SEM_HAS_TIMEDWAIT)
#endif /// !defined(POSIX_SEM_HAS_TIMEDWAIT)

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

#if !defined(POSIX_SEM_HAS_TIMEDWAIT)
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
#define POSIX_SEM_HAS_TIMEDWAIT
inline int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout) {
    return EINVAL;
}
#endif /// !defined(POSIX_SEM_HAS_TIMEDWAIT)

#if !defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
#if defined(SEM_HAS_TIMEDWAIT_RELATIVE_NP)
#define POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP
#endif /// defined(SEM_HAS_TIMEDWAIT_RELATIVE_NP)
#endif /// !defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)

#if !defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
#define POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP
inline int sem_timedwait_relative_np(sem_t *sem, const struct timespec *timeout) {
#if defined(POSIX_SEM_HAS_TIMEDWAIT)
    if ((sem) && (timeout)) {
        int err = 0; struct timespec untilTime;
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &untilTime))) {
            untilTime.tv_sec +=  timeout->tv_sec;
            untilTime.tv_nsec +=  timeout->tv_nsec;
            err = ::sem_timedwait(sem, &untilTime);
        }
        return err;
    }
#endif /// defined(POSIX_SEM_HAS_TIMEDWAIT)
    return EINVAL;
}
#endif /// !defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)

namespace xos {
namespace mt {
namespace posix {

///////////////////////////////////////////////////////////////////////
/// Enum: sem_pshared_t
///////////////////////////////////////////////////////////////////////
typedef int sem_pshared_t;
enum {
    SEMAPHORE_PROCESS_PRIVATE = 0,
    SEMAPHORE_PROCESS_SHARED  = 1
};

///////////////////////////////////////////////////////////////////////
///  Class: SemaphoreT
///////////////////////////////////////////////////////////////////////
template 
<typename TError = int, TError VErrorSuccess = 0, TError VErrorFailed = -1,
 typename TAttached = sem_t*, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::SemaphoreT<TAttached, TUnattached, VUnattached>,
 class TSemaphore = extended::SemaphoreT
 <TError, VErrorSuccess, VErrorFailed, TAttached, TUnattached, VUnattached, TImplement>,
 class TImplements = TImplement, class TExtends = TSemaphore>

class _EXPORTED_ SemaphoreT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Extends::Attached Attached;
    static const typename Extends::UnattachedT Unattached = Extends::Unattached;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    SemaphoreT(Attached detached, bool isCreated, bool isLogged, bool isErrLogged): Extends(detached, isCreated, isLogged, isErrLogged) {
    }
    SemaphoreT(Attached detached, bool isCreated, bool isLogged): Extends(detached, isCreated, isLogged) {
    }
    SemaphoreT(Attached detached, bool isCreated): Extends(detached, isCreated) {
    }
    SemaphoreT(Attached detached): Extends(detached) {
    }
    SemaphoreT(bool isLogged, bool isErrLogged): Extends(isLogged, isErrLogged) {
        XOS_MT_SEMAPHORE_CREATED();
    }
    SemaphoreT(bool isLogged): Extends(isLogged) {
        XOS_MT_SEMAPHORE_CREATED();
    }
    SemaphoreT() {
        XOS_MT_SEMAPHORE_CREATED();
    }
    SemaphoreT(const SemaphoreT &copy): Extends(copy.AttachedTo(), false) {
    }
    virtual ~SemaphoreT() {
        XOS_MT_SEMAPHORE_DESTROYED();
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Attached CreateAttached() {
        return CreateAttached(0);
    }
    virtual Attached CreateAttached(size_t initiallyReleased) {
        Attached detached = ((Attached)Unattached);
        if ((detached = CreateDetached(_sem, initiallyReleased))) {
            this->Attach(detached);
        }
        return detached;
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual AcquireStatus TimedAcquire(mseconds_t milliseconds) { 
        return TimedAcquireDetached(this->AttachedTo(), milliseconds); 
    }
    virtual AcquireStatus TryAcquire() { 
        return TryAcquireDetached(this->AttachedTo()); 
    }
    virtual AcquireStatus UntimedAcquire() { 
        return UntimedAcquireDetached(this->AttachedTo()); 
    }
    virtual AcquireStatus UntimedRelease() { 
        return UntimedReleaseDetached(this->AttachedTo()); 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual AcquireStatus TimedAcquireDetached(Attached detached, mseconds_t milliseconds) const { 
        if (0 < (milliseconds)) {
#if defined(POSIX_SEM_HAS_TIMEDWAIT)
            if (((Attached)Unattached) != detached) {
                mseconds_t millisecondsThreashold = this->TimedLoggedThreasholdMilliseconds();
                bool isLogged = ((this->IsLogged()) && (milliseconds >= millisecondsThreashold));
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

                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "sem_timedwait(detached, &untilTime)...");
                if (!(err = sem_timedwait(detached, &untilTime))) {
                    IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...sem_timedwait(detached, &untilTime)...");
                    return AcquireSuccess;
                } else {
                    err = errno;
                    if (EAGAIN == (err)) {
                        IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...failed EAGAIN err = " << err << " on sem_timedwait(detached, &untilTime)");
                        return AcquireBusy;
                    } else {
                        if (EBUSY == (err)) {
                            IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...failed EBUSY err = " << err << " on sem_timedwait(detached, &untilTime)");
                            return AcquireBusy;
                        } else {
                            if (ETIMEDOUT == (err)) {
                                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...failed ETIMEDOUT err = " << err << " on sem_timedwait(detached, &untilTime)");
                                return AcquireBusy;
                            } else {
                                if (EINTR == (err)) {
                                    IS_ERR_LOGGED_ERROR("...failed EINTR err = " << err << " on sem_timedwait(detached, &untilTime)");
                                    return AcquireInterrupted;
                                } else {
                                    IS_ERR_LOGGED_ERROR("...failed err = " << err << " on sem_timedwait(detached, &untilTime)");
                                }
                            }
                        }
                    }
                }
            }
#else /// defined(POSIX_SEM_HAS_TIMEDWAIT)
            IS_ERR_LOGGED_ERROR("...return AcquireInvalid");
            return AcquireInvalid;
#endif /// defined(POSIX_SEM_HAS_TIMEDWAIT)
        } else {
            if (0 > (milliseconds)) {
                return UntimedAcquireDetached(detached);
            } else {
                return TryAcquireDetached(detached);
            }
        }
        return AcquireFailed; 
    }
    virtual AcquireStatus TryAcquireDetached(Attached detached) const { 
        if (((Attached)Unattached) != detached) {
            int err = 0;
            IS_ERR_LOGGED_TRACE("::sem_trywait(detached)...");
            if (!(err = ::sem_trywait(detached))) {
                IS_ERR_LOGGED_TRACE("...::sem_trywait(detached)");
                return AcquireSuccess;
            } else {
                err = errno;
                if (EAGAIN == (err)) {
                    IS_ERR_LOGGED_TRACE("...failed EAGAIN err = " << err << " on ::sem_trywait(detached)");
                    return AcquireBusy;
                } else {
                    if (EBUSY == (err)) {
                        IS_ERR_LOGGED_TRACE("...failed EBUSY err = " << err << " on ::sem_trywait(detached)");
                        return AcquireBusy;
                    } else {
                        if (ETIMEDOUT == (err)) {
                            IS_ERR_LOGGED_TRACE("...failed ETIMEDOUT err = " << err << " on ::sem_trywait(detached)");
                            return AcquireBusy;
                        } else {
                            if (EINTR == (err)) {
                                IS_ERR_LOGGED_ERROR("...failed EINTR err = " << err << " on ::sem_trywait(detached)");
                                return AcquireInterrupted;
                            } else {
                                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sem_trywait(detached)");
                            }
                        }
                    }
                }
            }
        }
        return AcquireFailed; 
    }
    virtual AcquireStatus UntimedAcquireDetached(Attached detached) const { 
        if (((Attached)Unattached) != detached) {
            int err = 0;
            IS_ERR_LOGGED_DEBUG("::sem_wait(detached)...");
            if (!(err = ::sem_wait(detached))) {
                IS_ERR_LOGGED_DEBUG("...::sem_wait(detached)");
                return AcquireSuccess;
            } else {
                err = errno;
                if (EAGAIN == (err)) {
                    IS_ERR_LOGGED_ERROR("...failed EAGAIN err = " << err << " on ::sem_wait(detached)");
                    return AcquireFailed;
                } else {
                    if (EBUSY == (err)) {
                        IS_ERR_LOGGED_ERROR("...failed EBUSY err = " << err << " on ::sem_wait(detached)");
                        return AcquireFailed;
                    } else {
                        if (ETIMEDOUT == (err)) {
                            IS_ERR_LOGGED_ERROR("...failed ETIMEDOUT err = " << err << " on ::sem_wait(detached)");
                            return AcquireFailed;
                        } else {
                            if (EINTR == (err)) {
                                IS_ERR_LOGGED_ERROR("...failed EINTR err = " << err << " on ::sem_wait(detached)");
                                return AcquireInterrupted;
                            } else {
                                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sem_wait(detached)");
                            }
                        }
                    }
                }
            }
        }
        return AcquireFailed; 
    }
    virtual AcquireStatus UntimedReleaseDetached(Attached detached) const { 
        if (((Attached)Unattached) != detached) {
            int err = 0;

            IS_ERR_LOGGED_DEBUG("::sem_post(detached)...");
            if (!(err = ::sem_post(detached))) {
                IS_ERR_LOGGED_DEBUG("...::sem_post(detached)");
                return ReleaseSuccess;
            } else {
                err = errno;
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_signal(detached)");
            }
        }
        return ReleaseFailed; 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Attached CreateDetached(sem_t& sem) const {
        size_t initiallyReleased = 0;
        return CreateDetached(sem, initiallyReleased);
    }
    virtual Attached CreateDetached(sem_t& sem, size_t initiallyReleased) const {
        sem_pshared_t pshared = SEMAPHORE_PROCESS_PRIVATE;
        return CreateDetached(sem, pshared, initiallyReleased);
    }
    virtual Attached CreateDetached(sem_t& sem, sem_pshared_t pshared, size_t initiallyReleased) const {
        Attached detached = ((Attached)Unattached);
        int err = 0;
        IS_ERR_LOGGED_DEBUG("::sem_init(&sem, pshared, initiallyReleased)...");
        if ((!(err = ::sem_init(&sem, pshared, initiallyReleased)))) {
            IS_ERR_LOGGED_DEBUG("...::sem_init(&sem, pshared, initiallyReleased)");
            return &sem;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sem_init(&sem, pshared, initiallyReleased)");
        }
        return detached;
    }
    virtual bool DestroyDetached(Attached detached) const {
        if (((Attached)Unattached) != detached) {
            int err = 0;
            IS_ERR_LOGGED_DEBUG("::sem_destroy(detached)...");
            if (!(err = ::sem_destroy(detached))) {
                IS_ERR_LOGGED_DEBUG("...::sem_destroy(detached)");
                return true;
            } else {
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sem_destroy(detached)");
            }
        }
        return false;
    }
    
protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    inline int clock_gettime(clockid_t clk_id, struct timespec *res) const {
#if defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
        return ::clock_gettime_relative_np(res); 
#else defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
        return ::clock_gettime(clk_id, res); 
#endif /// defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
    }
    inline int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout) const {
#if defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
        return ::sem_timedwait_relative_np(sem, abs_timeout);
#else /// defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
        return ::sem_timedwait(sem. abs_timeout);
#endif /// defined(POSIX_SEM_HAS_TIMEDWAIT_RELATIVE_NP)
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    sem_t _sem;
}; /// class _EXPORTED_ SemaphoreT
typedef SemaphoreT<> Semaphore;

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_POSIX_SEMAPHORE_HXX_
