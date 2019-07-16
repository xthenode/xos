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
#if !defined(CWAIT_REALTIME)
#define CWAIT_REALTIME 0
#define clockid_t int
inline int clock_gettime(clockid_t clk_id, struct timespec *res) {
    if ((res)) {
        memset(res, 0, sizeof(struct timespec));
        return 0; 
    }
    return EINVAL; 
}
#else /// !defined(CWAIT_REALTIME)
#endif /// !defined(CWAIT_REALTIME)
inline int pthread_cond_timedlock(pthread_cond_t *cond, const struct timespec *abs_timeout) {
    return EINVAL; 
}
#define PTHREAD_CONDITION_HAS_TIMEDWAIT
#else /// !defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
#endif /// !defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)

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

    typedef typename Extends::Error Error;
    static const Error ErrorSuccess = Extends::ErrorSuccess;
    static const Error ErrorFailed = Extends::ErrorFailed;

    typedef typename Extends::Attached Attached;
    static const typename Extends::UnattachedT Unattached = Extends::Unattached;
    
    typedef SignalStatus (Derives::*WaitDetachedT)(pthread_cond_t&, pthread_mutex_t&, mseconds_t) const;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
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
    ConditionT(): _signaled(0), _mutex(0) {
        XOS_MT_CONDITION_CREATED();
    }
    virtual ~ConditionT() {
        XOS_MT_CONDITION_DESTROYED();
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
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
    virtual SignalStatus TryWait() { 
        SignalStatus status = TryTimedUntimedWait(&Derives::TimedWaitDetached, 0);
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
        bool isLogged = (milliseconds >= this->TimedLoggedThreasholdMilliseconds());
        Error error = 1;
        struct timespec untilTime;

#if defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "::memset(&untilTime, 0, sizeof(untilTime)...");
        if (!(::memset(&untilTime, 0, sizeof(untilTime)))) {
            IS_ERR_LOGGED_ERROR("...failed on ::memset(&untilTime, 0, sizeof(untilTime)");
            return WaitFailed;
        } else {
            IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "...::memset(&untilTime, 0, sizeof(untilTime)");
        }
#else /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "::clock_gettime(CLOCK_REALTIME, &untilTime)...");
        if ((error = ::clock_gettime(CLOCK_REALTIME, &untilTime))) {
            IS_ERR_LOGGED_ERROR("...failed " << error << " on ::clock_gettime(CLOCK_REALTIME, &untilTime)");
            return WaitFailed;
        } else {
            IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "...::clock_gettime(CLOCK_REALTIME, &untilTime)");
        }
#endif /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        untilTime.tv_sec +=  MSecondsSeconds(milliseconds);
        untilTime.tv_nsec +=  MSecondsNSeconds(MSecondsMSeconds(milliseconds));

#if defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "::pthread_cond_timedwait_relative_np(&cond, &mutex, &untilTime)...");
        if ((error = ::pthread_cond_timedwait_relative_np(&cond, &mutex, &untilTime))) {
            switch(error) {
            case ETIMEDOUT:
                IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "...ETIMEDOUT error = "<< error << " on ::pthread_cond_timedwait_relative_np(&cond, &mutex, &untilTime)");
                return WaitBusy;
            case EINTR:
                IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "...EINTR error = "<< error << " on ::pthread_cond_timedwait_relative_np(&cond, &mutex, &untilTime)");
                return WaitInterrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed error = "<< error << " on ::pthread_cond_timedwait_relative_np(&cond, &mutex, &untilTime)");
                return WaitFailed;
            }
        } else {
            IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "...::pthread_cond_timedwait_relative_np(&cond, &mutex, &untilTime)");
            return WaitSuccess;
        }
#else /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
        IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "::pthread_cond_timedwait(&cond, &mutex, &untilTime)...");
        if ((error = ::pthread_cond_timedwait(&cond, &mutex, &untilTime))) {
            switch(error) {
            case ETIMEDOUT:
                IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "...ETIMEDOUT error = "<< error << " on ::pthread_cond_timedwait(&cond, &mutex, &untilTime)");
                return WaitBusy;
            case EINTR:
                IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "...EINTR error = "<< error << " on ::pthread_cond_timedwait(&cond, &mutex, &untilTime)");
                return WaitInterrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed error = "<< error << " on ::pthread_cond_timedwait(&cond, &mutex, &untilTime)");
                return WaitFailed;
            }
        } else {
            IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "...::pthread_cond_timedwait(&cond, &mutex, &untilTime)");
            return WaitSuccess;
        }
#endif /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT_RELATIVE_NP)
#else /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
        IS_ERR_LOGGED_ERROR("...invalid on ::pthread_cond_timedwait(&cond, &mutex, &untilTime)");
        status = WaitInvalid;
#endif /// defined(PTHREAD_CONDITION_HAS_TIMEDWAIT)
        return status; 
    }
    virtual SignalStatus UntimedWaitDetached(pthread_cond_t& cond, pthread_mutex_t& mutex, mseconds_t milliseconds) const { 
        Error error = 1;
        IS_LOGGED_DEBUG("::pthread_cond_wait(&cond, &mutex)...");
        if ((error = ::pthread_cond_wait(&cond, &mutex))) {
            switch(error) {
            case EBUSY:
                IS_ERR_LOGGED_ERROR("...EBUSY error = "<< error << " on ::pthread_cond_wait(&cond, &mutex)");
                return WaitBusy;
            case ETIMEDOUT:
                IS_ERR_LOGGED_ERROR("...ETIMEDOUT error = "<< error << " on ::pthread_cond_wait(&cond, &mutex)");
                return WaitBusy;
            case EINTR:
                IS_ERR_LOGGED_ERROR("...EINTR error = "<< error << " on ::pthread_cond_wait(&cond, &mutex)");
                return WaitInterrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed error = "<< error << " on ::pthread_cond_wait(&cond, &mutex)");
                return WaitFailed;
            }
        } else {
            return WaitSuccess;
        }
        return WaitFailed; 
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
        Error error = 1;
        IS_LOGGED_DEBUG("::pthread_cond_signal(&cond)...");
        if (!(error = ::pthread_cond_signal(&cond))) {
            IS_LOGGED_DEBUG("...::pthread_cond_signal(&cond)");
            return true;
        } else {
            IS_ERR_LOGGED_ERROR("...failed error = "<< error << " on ::pthread_cond_signal(&cond)");
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
        Error error = 0;
        pthread_condattr_t condattr;

        IS_ERR_LOGGED_DEBUG("::pthread_condattr_init(&condattr)...");
        if (!(error = ::pthread_condattr_init(&condattr))) {
            IS_ERR_LOGGED_DEBUG("...::pthread_condattr_init(&condattr)");
            IS_ERR_LOGGED_DEBUG("::pthread_cond_init(&cond, &condattr)...");
            if (!(error = ::pthread_cond_init(&cond, &condattr))) {
                IS_ERR_LOGGED_DEBUG("...::pthread_cond_init(&cond, &condattr)");
                detached = &cond;
            } else {
                IS_ERR_LOGGED_ERROR("...failed error = "<< error << " on ::pthread_cond_init(&cond, &condattr)");
            }
            IS_ERR_LOGGED_DEBUG("::pthread_condattr_destroy(&condattr)...");
            if ((error = ::pthread_condattr_destroy(&condattr))) {
                IS_ERR_LOGGED_ERROR("...failed error = "<< error << " on ::pthread_condattr_destroy(&condattr)");
                detached = ((Attached)Unattached);
                IS_ERR_LOGGED_DEBUG("::pthread_cond_destroy(&cond)...");
                if ((error = ::pthread_cond_destroy(&cond))) {
                    IS_ERR_LOGGED_ERROR("...failed error = "<< error << " on ::pthread_cond_destroy(&cond)");
                } else {
                    IS_ERR_LOGGED_DEBUG("...::pthread_cond_destroy(&cond)");
                }
            } else {
                IS_ERR_LOGGED_DEBUG("...::pthread_condattr_destroy(&condattr)");
            }
        } else {
            IS_ERR_LOGGED_ERROR("...failed error = "<< error << " on ::pthread_condattr_init(&condattr)");
        }
        return detached;
    }
    virtual bool DestroyDetached(Attached detached) const {
        if (((Attached)Unattached) != detached) {
            Error error = 1;
            IS_ERR_LOGGED_DEBUG("::pthread_cond_destroy(detached)...");
            if ((ErrorSuccess != (error = ::pthread_cond_destroy(detached)))) {
                IS_ERR_LOGGED_ERROR("...failed error = "<< error << " ::pthread_cond_destroy(detached)");
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
