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
///   File: Condition.hxx
///
/// Author: $author$
///   Date: 7/4/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_POSIX_CONDITION_HXX_
#define _XOS_MT_POSIX_CONDITION_HXX_

#include "xos/mt/Condition.hxx"
#include "xos/mt/posix/Mutex.hxx"

#include <pthread.h>
#include <time.h>
#include <errno.h>

#if !defined(HAS_POSIX_TIMEOUTS)
#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#define HAS_POSIX_TIMEOUTS
#else /// defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#endif /// defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#endif /// !defined(HAS_POSIX_TIMEOUTS)

#if defined(HAS_POSIX_TIMEOUTS)
#if !defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
#define PTHREAD_CONDITION_HAS_TIMEDWAIT
#endif /// !defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
#endif /// defined(HAS_POSIX_TIMEOUTS)

#if !defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
#if !defined(CLOCK_REALTIME)
#define CLOCK_REALTIME 0
#define clockid_t int
inline int clock_gettime(clockid_t clk_id, struct timespec *res) {
    if ((res)) {
        memset(res, 0, sizeof(struct timespec));
        return 0; 
    }
    return EINVAL; 
}
#else /// !defined(CLOCK_REALTIME)
#endif /// !defined(CLOCK_REALTIME)
inline int pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abstime) {
    return EINVAL; 
}
#define PTHREAD_CONDITION_HAS_TIMEDWAIT
#else /// !defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
#endif /// !defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)

#if !defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
inline int pthread_cond_timedwait_relative_np(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* reltime) {
#if defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
    if ((cond) && (mutex) && (reltime)) {
        int err = 0; struct timespec untilTime;
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &untilTime))) {
            untilTime.tv_sec +=  reltime->tv_sec;
            untilTime.tv_nsec +=  reltime->tv_nsec;
            err = ::pthread_cond_timedwait(cond, mutex, &untilTime);
        }
        return err;
    }
#endif /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
    return EINVAL; 
}
#define PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP
#endif /// !defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)

#if !defined(PTHREAD_CONDITION_HAS_TRYWAIT)
inline int pthread_cond_trywait(pthread_cond_t* cond, pthread_mutex_t* mutex) {
#if defined(PTHREAD_CONDITION_HAS_TIMEDWAIT) || defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
    if ((cond) && (mutex)) {
        int err = 0; struct timespec untilTime;
#if defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        ::memset(&untilTime, 0, sizeof(untilTime));
        err = ::pthread_cond_timedwait_relative_np(cond, mutex, &untilTime);
#else /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &untilTime))) {
            err = ::pthread_cond_timedwait(cond, mutex, &untilTime);
        }
#endif /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        return err;
    }
#endif /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT) || defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
    return EINVAL;
}
#define PTHREAD_CONDITION_HAS_TRYWAIT
#endif /// !defined(PTHREAD_CONDITION_HAS_TRYWAIT)

namespace xos {
namespace mt {
namespace posix {

///////////////////////////////////////////////////////////////////////
///  Class: ConditionT
///////////////////////////////////////////////////////////////////////
template 
<typename TError = int, TError VErrorSuccess = 0, TError VErrorFailed = -1,
 typename TAttached = pthread_cond_t*, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::ConditionT<TAttached, TUnattached, VUnattached>,
 class TCondition = extended::ConditionT
 <TError, VErrorSuccess, VErrorFailed, TAttached, TUnattached, VUnattached, TImplement>,
 class TImplements = TImplement, class TExtends = TCondition>

class _EXPORT_CLASS ConditionT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef ConditionT Derives;

    typedef pthread_mutex_t* MutexAttached;
    typedef typename Extends::Attached Attached;
    static const typename Extends::UnattachedT Unattached = Extends::Unattached;
    
    typedef SignalStatus (Derives::*WaitDetachedT)(pthread_cond_t&, pthread_mutex_t&, mseconds_t) const;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ConditionT(Attached detached, bool isCreated, bool isLogged, bool isErrLogged, pthread_mutex_t* mutex, bool& signaled): Extends(detached, isCreated, isLogged, isErrLogged), _signaled(&signaled), _mutex(mutex) {
    }
    ConditionT(Attached detached, bool isCreated, bool isLogged, pthread_mutex_t* mutex, bool& signaled): Extends(detached, isCreated, isLogged), _signaled(&signaled), _mutex(mutex) {
    }
    ConditionT(Attached detached, bool isCreated, pthread_mutex_t* mutex, bool& signaled): Extends(detached, isCreated), _signaled(&signaled), _mutex(mutex) {
    }
    ConditionT(Attached detached, pthread_mutex_t* mutex, bool& signaled): Extends(detached), _signaled(&signaled), _mutex(mutex) {
    }
    ConditionT(Attached detached, bool isCreated, bool isLogged, bool isErrLogged): Extends(detached, isCreated, isLogged, isErrLogged), _signaled(0), _mutex(0) {
    }
    ConditionT(Attached detached, bool isCreated, bool isLogged): Extends(detached, isCreated, isLogged), _signaled(0), _mutex(0) {
    }
    ConditionT(Attached detached, bool isCreated): Extends(detached, isCreated), _signaled(0), _mutex(0) {
    }
    ConditionT(Attached detached): Extends(detached), _signaled(0), _mutex(0) {
    }
    ConditionT(bool isLogged, bool isErrLogged): Extends(isLogged, isErrLogged), _signaled(0), _mutex(0) {
        XOS_MT_CONDITION_CREATED();
    }
    ConditionT(bool isLogged): Extends(isLogged), _signaled(0), _mutex(0) {
        XOS_MT_CONDITION_CREATED();
    }
    ConditionT(const ConditionT& copy): Extends(copy), _signaled(0), _mutex(0) {
    }
    ConditionT(Mutex& mutex, bool& signaled): _signaled(&signaled), _mutex(mutex.AttachedTo()) {
        XOS_MT_CONDITION_CREATED();
    }
    ConditionT(pthread_mutex_t* mutex, bool& signaled): _signaled(&signaled), _mutex(mutex) {
        XOS_MT_CONDITION_CREATED();
    }
    ConditionT(): _signaled(0), _mutex(0) {
        XOS_MT_CONDITION_CREATED();
    }
    virtual ~ConditionT() {
        XOS_MT_CONDITION_DESTROYED();
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool Broadcast() { 
        bool success = false;
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->AttachedTo())) {
            bool* signaled = 0;
            if ((0 != (signaled = _signaled))) {
                pthread_mutex_t* mutex = 0;
                if ((0 != (mutex = _mutex))) {
                    success = BroadcastDetached(*detached, *mutex, *signaled);
                } else {
                    IS_ERR_LOGGED_ERROR("...failed 0 = (_mutex)");
                }
            } else {
                IS_ERR_LOGGED_ERROR("...failed 0 = (_signaled)");
            }
        }
        return success; 
    }
    virtual bool Signal() { 
        bool success = false;
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->AttachedTo())) {
            bool* signaled = 0;
            if ((0 != (signaled = _signaled))) {
                pthread_mutex_t* mutex = 0;
                if ((0 != (mutex = _mutex))) {
                    success = SignalDetached(*detached, *mutex, *signaled);
                } else {
                    IS_ERR_LOGGED_ERROR("...failed 0 = (_mutex)");
                }
            } else {
                IS_ERR_LOGGED_ERROR("...failed 0 = (_signaled)");
            }
        }
        return success; 
    }
    virtual bool Wait() { 
        if (WaitSuccess == (UntimedWait())) {
            return true;
        }
        return false; 
    }
    virtual SignalStatus TryWait() { 
        SignalStatus status = SignalFailed;
#if defined(PTHREAD_CONDITION_HAS_TRYWAIT)
        status = TryTimedUntimedWait(&Derives::TryWaitDetached, 0);
#else /// defined(PTHREAD_CONDITION_HAS_TRYWAIT)
        status = TryTimedUntimedWait(&Derives::TimedWaitDetached, 0);
#endif /// defined(PTHREAD_CONDITION_HAS_TRYWAIT)
        return status; 
    }
    virtual SignalStatus TimedWait(mseconds_t milliseconds) { 
        SignalStatus status = TryTimedUntimedWait(&Derives::TimedWaitDetached, milliseconds);
        return status; 
    }
    virtual SignalStatus UntimedWait() { 
        SignalStatus status = TryTimedUntimedWait(&Derives::UntimedWaitDetached, -1);
        return status; 
    }
    virtual SignalStatus TryTimedUntimedWait(WaitDetachedT waitDetached, mseconds_t milliseconds) { 
        SignalStatus status = WaitFailed;
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->AttachedTo())) {
            bool* signaled = 0;
            if ((0 != (signaled = _signaled))) {
                pthread_mutex_t* mutex = 0;
                if ((0 != (mutex = _mutex))) {
                    status = TryTimedUntimedWaitDetached(waitDetached, *detached, *mutex, *signaled, milliseconds);
                } else {
                    IS_ERR_LOGGED_ERROR("...failed 0 = (_mutex)");
                }
            } else {
                IS_ERR_LOGGED_ERROR("...failed 0 = (_signaled)");
            }
        }
        return status; 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool Wait(pthread_mutex_t& mutex) { 
        if (WaitSuccess == (UntimedWait(mutex))) {
            return true;
        }
        return false; 
    }
    virtual SignalStatus TimedWait(pthread_mutex_t& mutex, mseconds_t milliseconds) { 
        SignalStatus status = WaitFailed;
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->AttachedTo())) {
            status = TimedWaitDetached(*detached, mutex, milliseconds);
        }
        return status; 
    }
    virtual SignalStatus TryWait(pthread_mutex_t& mutex) { 
        SignalStatus status = WaitFailed;
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->AttachedTo())) {
            status = TryWaitDetached(*detached, mutex, 0);
        }
        return status; 
    }
    virtual SignalStatus UntimedWait(pthread_mutex_t& mutex) { 
        SignalStatus status = WaitFailed;
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->AttachedTo())) {
            status = UntimedWaitDetached(*detached, mutex, 0);
        }
        return status; 
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual SignalStatus TryTimedUntimedWaitDetached(WaitDetachedT waitDetached, pthread_cond_t& cond, pthread_mutex_t& mutex, bool& signaled, mseconds_t milliseconds) const { 
        SignalStatus status = WaitFailed;
        if ((0 != (waitDetached))) {
            for (bool unsignaled = true; unsignaled; ) {
                try {
                    Mutex locked(&mutex);
                    ::xos::Lock lock(locked);
                    if (!(unsignaled = !signaled)) {
                        status = WaitSuccess;
                    } else {
                        lock.SetUnlocked();
                        if (WaitSuccess != (status = (this->*waitDetached)(cond, mutex, milliseconds))) {
                            lock.SetUnlocked(false);
                            unsignaled = false;
                        }
                    }
                } catch (...) {
                    IS_ERR_LOGGED_ERROR("...catch(...)");
                    unsignaled = false;
                }
            }
        } else {
            IS_ERR_LOGGED_ERROR("...failed 0 = (waitDetached)");
        }
        return status; 
    }
    virtual SignalStatus TimedWaitDetached(pthread_cond_t& cond, pthread_mutex_t& mutex, mseconds_t milliseconds) const { 
        SignalStatus status = WaitFailed;
#if defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
        bool isLogged = ((this->IsLogged()) && (milliseconds >= this->TimedLoggedThreasholdMilliseconds()));
        int err = 0;
        struct timespec untilTime;

#if defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "::memset(&untilTime, 0, sizeof(untilTime)...");
        if (!(::memset(&untilTime, 0, sizeof(untilTime)))) {
            IS_ERR_LOGGED_ERROR("...failed on ::memset(&untilTime, 0, sizeof(untilTime)");
            return WaitFailed;
        } else {
            IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...::memset(&untilTime, 0, sizeof(untilTime)");
        }
#else /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "::clock_gettime(CLOCK_REALTIME, &untilTime)...");
        if ((err = ::clock_gettime(CLOCK_REALTIME, &untilTime))) {
            IS_ERR_LOGGED_ERROR("...failed " << err << " on ::clock_gettime(CLOCK_REALTIME, &untilTime)");
            return WaitFailed;
        } else {
            IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...::clock_gettime(CLOCK_REALTIME, &untilTime)");
        }
#endif /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        untilTime.tv_sec +=  MSecondsSeconds(milliseconds);
        untilTime.tv_nsec +=  MSecondsNSeconds(MSecondsMSeconds(milliseconds));

#if defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "::pthread_cond_timedwait_relative_np(&cond, &mutex, &untilTime)...");
        if ((err = ::pthread_cond_timedwait_relative_np(&cond, &mutex, &untilTime))) {
            switch(err) {
            case ETIMEDOUT:
                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...ETIMEDOUT err = "<< err << " on ::pthread_cond_timedwait_relative_np(&cond, &mutex, &untilTime)");
                return WaitBusy;
            case EINTR:
                IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_cond_timedwait_relative_np(&cond, &mutex, &untilTime)");
                return WaitInterrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_timedwait_relative_np(&cond, &mutex, &untilTime)");
                return WaitFailed;
            }
        } else {
            IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...::pthread_cond_timedwait_relative_np(&cond, &mutex, &untilTime)");
            return WaitSuccess;
        }
#else /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "::pthread_cond_timedwait(&cond, &mutex, &untilTime)...");
        if ((err = ::pthread_cond_timedwait(&cond, &mutex, &untilTime))) {
            switch(err) {
            case ETIMEDOUT:
                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...ETIMEDOUT err = "<< err << " on ::pthread_cond_timedwait(&cond, &mutex, &untilTime)");
                return WaitBusy;
            case EINTR:
                IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_cond_timedwait(&cond, &mutex, &untilTime)");
                return WaitInterrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_timedwait(&cond, &mutex, &untilTime)");
                return WaitFailed;
            }
        } else {
            IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...::pthread_cond_timedwait(&cond, &mutex, &untilTime)");
            return WaitSuccess;
        }
#endif /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
#else /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
        IS_ERR_LOGGED_ERROR("...invalid on ::pthread_cond_timedwait(&cond, &mutex, &untilTime)");
        status = WaitInvalid;
#endif /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
        return status; 
    }
    virtual SignalStatus TryWaitDetached(pthread_cond_t& cond, pthread_mutex_t& mutex, mseconds_t milliseconds) const {
        SignalStatus status = WaitFailed;
#if defined(PTHREAD_CONDITION_HAS_TRYWAIT)
        int err = 0;
        IS_ERR_LOGGED_TRACE("::pthread_cond_trywait(&cond, &mutex)...");
        if ((err = ::pthread_cond_trywait(&cond, &mutex))) {
            switch(err) {
            case ETIMEDOUT:
                IS_ERR_LOGGED_TRACE("...ETIMEDOUT err = "<< err << " on ::pthread_cond_trywait(&cond, &mutex)");
                return WaitBusy;
            case EINTR:
                IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_cond_trywait(&cond, &mutex)");
                return WaitInterrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_trywait(&cond, &mutex)");
                return WaitFailed;
            }
        } else {
            IS_ERR_LOGGED_TRACE("...::pthread_cond_trywait(&cond, &mutex)");
            return WaitSuccess;
        }
#else /// defined(PTHREAD_CONDITION_HAS_TRYWAIT)
        IS_ERR_LOGGED_ERROR("...invalid on ::pthread_cond_trywait(&cond, &mutex)");
        status = WaitInvalid;
#endif /// defined(PTHREAD_CONDITION_HAS_TRYWAIT)
        return status;
    }
    virtual SignalStatus UntimedWaitDetached(pthread_cond_t& cond, pthread_mutex_t& mutex, mseconds_t milliseconds) const {
        int err = 0;
        IS_LOGGED_DEBUG("::pthread_cond_wait(&cond, &mutex)...");
        if ((err = ::pthread_cond_wait(&cond, &mutex))) {
            switch(err) {
            case EBUSY:
                IS_ERR_LOGGED_ERROR("...EBUSY err = "<< err << " on ::pthread_cond_wait(&cond, &mutex)");
                return WaitBusy;
            case ETIMEDOUT:
                IS_ERR_LOGGED_ERROR("...ETIMEDOUT err = "<< err << " on ::pthread_cond_wait(&cond, &mutex)");
                return WaitBusy;
            case EINTR:
                IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_cond_wait(&cond, &mutex)");
                return WaitInterrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_wait(&cond, &mutex)");
                return WaitFailed;
            }
        } else {
            return WaitSuccess;
        }
        return WaitFailed; 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool BroadcastDetached(pthread_cond_t& cond, pthread_mutex_t& mutex, bool& signaled) const { 
        bool success = false;
        try {
            Mutex locked(&mutex);
            ::xos::Lock lock(locked);
            signaled = true;
        } catch (...) {
            IS_ERR_LOGGED_ERROR("...catch(...)");
            return false;
        }
        success = BroadcastDetached(cond);
        return success;
    }
    virtual bool BroadcastDetached(pthread_cond_t& cond) const { 
        int err = 0;
        IS_LOGGED_DEBUG("::pthread_cond_broadcast(&cond)...");
        if (!(err = ::pthread_cond_broadcast(&cond))) {
            IS_LOGGED_DEBUG("...::pthread_cond_broadcast(&cond)");
            return true;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_broadcast(&cond)");
        }
        return false; 
    }
    virtual bool SignalDetached(pthread_cond_t& cond, pthread_mutex_t& mutex, bool& signaled) const { 
        bool success = false;
        try {
            Mutex locked(&mutex);
            ::xos::Lock lock(locked);
            signaled = true;
        } catch (...) {
            IS_ERR_LOGGED_ERROR("...catch(...)");
            return false;
        }
        success = SignalDetached(cond);
        return success;
    }
    virtual bool SignalDetached(pthread_cond_t& cond) const { 
        int err = 0;
        IS_LOGGED_DEBUG("::pthread_cond_signal(&cond)...");
        if (!(err = ::pthread_cond_signal(&cond))) {
            IS_LOGGED_DEBUG("...::pthread_cond_signal(&cond)");
            return true;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_signal(&cond)");
        }
        return false; 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Attached CreateAttached() {
        Attached detached = ((Attached)Unattached);
        if ((detached = CreateDetached(_cond))) {
            this->Attach(detached);
        }
        return detached;
    }
    virtual Attached CreateDetached(pthread_cond_t& cond) const {
        Attached detached = ((Attached)Unattached);
        int err = 0;
        pthread_condattr_t condattr;

        IS_ERR_LOGGED_DEBUG("::pthread_condattr_init(&condattr)...");
        if (!(err = ::pthread_condattr_init(&condattr))) {
            IS_ERR_LOGGED_DEBUG("...::pthread_condattr_init(&condattr)");
            IS_ERR_LOGGED_DEBUG("::pthread_cond_init(&cond, &condattr)...");
            if (!(err = ::pthread_cond_init(&cond, &condattr))) {
                IS_ERR_LOGGED_DEBUG("...::pthread_cond_init(&cond, &condattr)");
                detached = &cond;
            } else {
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_init(&cond, &condattr)");
            }
            IS_ERR_LOGGED_DEBUG("::pthread_condattr_destroy(&condattr)...");
            if ((err = ::pthread_condattr_destroy(&condattr))) {
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_condattr_destroy(&condattr)");
                detached = ((Attached)Unattached);
                IS_ERR_LOGGED_DEBUG("::pthread_cond_destroy(&cond)...");
                if ((err = ::pthread_cond_destroy(&cond))) {
                    IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_cond_destroy(&cond)");
                } else {
                    IS_ERR_LOGGED_DEBUG("...::pthread_cond_destroy(&cond)");
                }
            } else {
                IS_ERR_LOGGED_DEBUG("...::pthread_condattr_destroy(&condattr)");
            }
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_condattr_init(&condattr)");
        }
        return detached;
    }
    virtual bool DestroyDetached(Attached detached) const {
        if (((Attached)Unattached) != detached) {
            int err = 0;
            IS_ERR_LOGGED_DEBUG("::pthread_cond_destroy(detached)...");
            if (((err = ::pthread_cond_destroy(detached)))) {
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " ::pthread_cond_destroy(detached)");
            } else {
                IS_ERR_LOGGED_DEBUG("...::pthread_cond_destroy(detached)");
                return true;
            }
        }
        return false;
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    bool* _signaled;
    pthread_mutex_t* _mutex;
    pthread_cond_t _cond;
}; /// class _EXPORT_CLASS ConditionT
typedef ConditionT<> Condition;

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_POSIX_CONDITION_HXX_
