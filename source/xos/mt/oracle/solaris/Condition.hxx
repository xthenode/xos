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
///   Date: 7/29/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_ORACLE_SOLARIS_CONDITION_HXX_
#define _XOS_MT_ORACLE_SOLARIS_CONDITION_HXX_

#include "xos/mt/Condition.hxx"
#include "xos/mt/oracle/solaris/Mutex.hxx"

#if defined(SOLARIS)
#include <thread.h> 
#include <synch.h> 
#else /// defined(SOLARIS)
#include "xos/platform/os/oracle/solaris/synch.h"
#endif /// defined(SOLARIS)

#if !defined(COND_HAS_TIMEDWAIT)
#define COND_HAS_TIMEDWAIT
#endif /// !defined(COND_HAS_TIMEDWAIT)

#if !defined(COND_HAS_RELTIMEDWAIT)
#define COND_HAS_RELTIMEDWAIT
#endif /// !defined(COND_HAS_RELTIMEDWAIT)

#if !defined(COND_HAS_TRYWAIT)
inline int cond_trywait(cond_t *cvp, mutex_t *mp) {
#if defined(COND_HAS_TIMEDWAIT) || defined(COND_HAS_RELTIMEDWAIT)
    if ((cvp) && (mp)) {
        int err = 0;
        timestruc_t timeout;
#if defined(COND_HAS_RELTIMEDWAIT)
        ::memset(&timeout, 0, sizeof(timeout));
#else /// defined(COND_HAS_RELTIMEDWAIT)
        err = clock_gettime(CLOCK_REALTIME, &timeout);
#endif /// defined(COND_HAS_RELTIMEDWAIT)
        if (!(err)) {
            return cond_timedwait(cvp, mp, &timeout);
        }
#endif /// defined(COND_HAS_TIMEDWAIT) || defined(COND_HAS_RELTIMEDWAIT)
    }
    return EINVAL;
}
#define COND_HAS_TRYWAIT
#endif /// !defined(COND_HAS_TRYWAIT)

namespace xos {
namespace mt {
namespace oracle {
namespace solaris {

class _EXPORTED_ ConditionFriend;
///////////////////////////////////////////////////////////////////////
///  Class: ConditionT
///////////////////////////////////////////////////////////////////////
template 
<typename TError = int, TError VErrorSuccess = 0, TError VErrorFailed = -1,
 typename TAttached = cond_t*, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::ConditionT<TAttached, TUnattached, VUnattached>,
 class TCondition = extended::ConditionT
 <TError, VErrorSuccess, VErrorFailed, TAttached, TUnattached, VUnattached, TImplement>,
 class TImplements = TImplement, class TExtends = TCondition>

class _EXPORTED_ ConditionT: virtual public TImplements, public TExtends {
public:
    friend class ConditionFriend;
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef ConditionT Derives;

    typedef mutex_t* MutexAttached;
    typedef typename Extends::Attached Attached;
    static const typename Extends::UnattachedT Unattached = Extends::Unattached;
    
    typedef SignalStatus (Derives::*WaitDetachedT)(cond_t&, mutex_t&, mseconds_t) const;

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
                mutex_t* mutex = 0;
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
        SignalStatus status = SignalFailed;
#if defined(COND_HAS_TRYWAIT)
        status = TryTimedUntimedWait(&Derives::TryWaitDetached, 0);
#else /// defined(COND_HAS_TRYWAIT)
        status = TryTimedUntimedWait(&Derives::TimedWaitDetached, 0);
#endif /// defined(COND_HAS_TRYWAIT)
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
                mutex_t* mutex = 0;
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
    virtual SignalStatus TryTimedUntimedWaitDetached(WaitDetachedT waitDetached, cond_t& cond, mutex_t& mutex, bool& signaled, mseconds_t milliseconds) const { 
        SignalStatus status = WaitFailed;
        if ((0 != (waitDetached))) {
            for (bool unsignaled = true; unsignaled; ) {
                try {
                    Mutex locked(&mutex);
                    Lock lock(locked);
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
    virtual SignalStatus TryWaitDetached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const {
        SignalStatus status = WaitFailed;
#if defined(COND_HAS_TRYWAIT)
        int err = 0;
        IS_ERR_LOGGED_TRACE("::cond_trywait(&cond, &mutex)...");
        if ((err = ::cond_trywait(&cond, &mutex))) {
            switch(err) {
            case ETIME:
                IS_ERR_LOGGED_TRACE("...ETIME err = "<< err << " on ::cond_trywait(&cond, &mutex)");
                return WaitBusy;
            case EINTR:
                IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::cond_trywait(&cond, &mutex)");
                return WaitInterrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::cond_trywait(&cond, &mutex)");
                return WaitFailed;
            }
        } else {
            IS_ERR_LOGGED_TRACE("...::cond_trywait(&cond, &mutex)");
            return WaitSuccess;
        }
#else /// defined(COND_HAS_TRYWAIT)
        IS_ERR_LOGGED_ERROR("...invalid on ::cond_trywait(&cond, &mutex)");
        status = WaitInvalid;
#endif /// defined(COND_HAS_TRYWAIT)
        return status;
    }
    virtual SignalStatus TimedWaitDetached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const { 
        SignalStatus status = WaitFailed;
#if defined(COND_HAS_TIMEDWAIT)
        bool isLogged = ((this->IsLogged()) && (milliseconds >= this->TimedLoggedThreasholdMilliseconds()));
        int err = 0;
        timestruc_t untilTime;

#if defined(COND_HAS_RELTIMEDWAIT)
        IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "::memset(&untilTime, 0, sizeof(untilTime)...");
        ::memset(&untilTime, 0, sizeof(untilTime));
#else /// defined(COND_HAS_RELTIMEDWAIT)
        IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "::clock_gettime(CLOCK_REALTIME, &untilTime)...");
        if ((err = ::clock_gettime(CLOCK_REALTIME, &untilTime))) {
            IS_ERR_LOGGED_ERROR("...failed " << err << " on ::clock_gettime(CLOCK_REALTIME, &untilTime)");
            return WaitFailed;
        }
#endif /// defined(COND_HAS_RELTIMEDWAIT)

        untilTime.tv_sec +=  MSecondsSeconds(milliseconds);
        untilTime.tv_nsec +=  MSecondsNSeconds(MSecondsMSeconds(milliseconds));

#if defined(COND_HAS_RELTIMEDWAIT)
        IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "::cond_reltimedwait(&cond, &mutex, &untilTime)...");
        if ((err = ::cond_reltimedwait(&cond, &mutex, &untilTime))) {
            switch(err) {
            case ETIME:
                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...ETIME err = "<< err << " on ::cond_reltimedwait(&cond, &mutex, &untilTime)");
                return WaitBusy;
            case EINTR:
                IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::cond_reltimedwait(&cond, &mutex, &untilTime)");
                return WaitInterrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::cond_reltimedwait(&cond, &mutex, &untilTime)");
                return WaitFailed;
            }
        } else {
            IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...::cond_reltimedwait(&cond, &mutex, &untilTime)");
            return WaitSuccess;
        }
#else /// defined(COND_HAS_RELTIMEDWAIT)
        IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "::cond_timedwait(&cond, &mutex, &untilTime)...");
        if ((err = ::cond_timedwait(&cond, &mutex, &untilTime))) {
            switch(err) {
            case ETIME:
                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...ETIME err = "<< err << " on ::cond_timedwait(&cond, &mutex, &untilTime)");
                return WaitBusy;
            case EINTR:
                IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::cond_timedwait(&cond, &mutex, &untilTime)");
                return WaitInterrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::cond_timedwait(&cond, &mutex, &untilTime)");
                return WaitFailed;
            }
        } else {
            IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...::cond_timedwait(&cond, &mutex, &untilTime)");
            return WaitSuccess;
        }
#endif /// defined(COND_HAS_RELTIMEDWAIT)
#else /// defined(COND_HAS_TIMEDWAIT)
        IS_ERR_LOGGED_ERROR("...invalid on ::cond_timedwait(&cond, &mutex, &untilTime)");
        status = WaitInvalid;
#endif /// defined(COND_HAS_TIMEDWAIT)
        return status;
    }
    virtual SignalStatus UntimedWaitDetached(cond_t& cond, mutex_t& mutex, mseconds_t milliseconds) const {
        int err = 0;
        IS_LOGGED_DEBUG("::cond_wait(&cond, &mutex)...");
        if ((err = ::cond_wait(&cond, &mutex))) {
            switch(err) {
            case ETIME:
                IS_ERR_LOGGED_ERROR("...ETIME err = "<< err << " on ::cond_wait(&cond, &mutex)");
                return WaitBusy;
            case EINTR:
                IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::cond_wait(&cond, &mutex)");
                return WaitInterrupted;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::cond_wait(&cond, &mutex)");
                return WaitFailed;
            }
        } else {
            return WaitSuccess;
        }
        return WaitFailed; 
    }
    virtual bool SignalDetached(cond_t& cond, mutex_t& mutex, bool& signaled) const { 
        bool success = false;
        try {
            Mutex locked(&mutex);
            Lock lock(locked);
            signaled = true;
        } catch (...) {
            IS_ERR_LOGGED_ERROR("...catch(...)");
            return false;
        }
        success = SignalDetached(cond);
        return success;
    }
    virtual bool SignalDetached(cond_t& cond) const { 
        int err = 0;
        IS_LOGGED_DEBUG("::cond_signal(&cond)...");
        if (!(err = ::cond_signal(&cond))) {
            IS_LOGGED_DEBUG("...::cond_signal(&cond)");
            return true;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::cond_signal(&cond)");
        }
        return false; 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Attached CreateAttached() {
        Attached detached = ((Attached)Unattached);
        if ((detached = CreateDetached(_cond, this))) {
            this->Attach(detached);
        }
        return detached;
    }
    virtual Attached CreateDetached(cond_t& cond, void* arg) const {
        Attached detached = ((Attached)Unattached);
        int err = 0;
        int type = USYNC_THREAD;
        IS_ERR_LOGGED_DEBUG("::cond_init(&cond, type, arg)...");
        if (!(err = ::cond_init(&cond, type, arg))) {
            IS_ERR_LOGGED_DEBUG("...::cond_init(&cond, type, arg)");
            detached = &cond;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::cond_init(&cond, type, arg)");
        }
        return detached;
    }
    virtual bool DestroyDetached(Attached detached) const {
        if (((Attached)Unattached) != detached) {
            int err = 0;
            IS_ERR_LOGGED_DEBUG("::cond_destroy(detached)...");
            if (((err = ::cond_destroy(detached)))) {
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " ::cond_destroy(detached)");
            } else {
                IS_ERR_LOGGED_DEBUG("...::cond_destroy(detached)");
                return true;
            }
        }
        return false;
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    bool* _signaled;
    mutex_t* _mutex;
    cond_t _cond;
}; /// class _EXPORTED_ ConditionT
typedef ConditionT<> Condition;

} /// namespace solaris
} /// namespace oracle
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_ORACLE_SOLARIS_CONDITION_HXX_
