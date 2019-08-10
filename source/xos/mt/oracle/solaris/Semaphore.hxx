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
///   Date: 7/23/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_ORACLE_SOLARIS_SEMAPHORE_HXX_
#define _XOS_MT_ORACLE_SOLARIS_SEMAPHORE_HXX_

#include "xos/mt/Semaphore.hxx"

#if defined(SOLARIS)
#include <synch.h>
#else /// defined(SOLARIS)
#include "xos/platform/os/oracle/solaris/semaphore.h"
#endif /// defined(SOLARIS)

#if !defined(SEMA_HAS_TIMEDWAIT)
#define SEMA_HAS_TIMEDWAIT
#endif /// !defined(SEMA_HAS_TIMEDWAIT)

#if !defined(SEMA_HAS_RELTIMEDWAIT)
#define SEMA_HAS_RELTIMEDWAIT
#endif /// !defined(SEMA_HAS_RELTIMEDWAIT)

#if !defined(CLOCK_HAS_RELGETTIME)
inline int clock_relgettime(timestruc_t* abstime) {
    if ((abstime)) {
        abstime->tv_sec = 0;
        abstime->tv_nsec = 0;
        return 0;
    }
    return EINVAL;
}
#define CLOCK_HAS_RELGETTIME
#endif /// !defined(CLOCK_HAS_RELGETTIME)

#if defined(SEMA_HAS_TIMEDWAIT)
#if !defined(CLOCK_REALTIME)
#define CLOCK_REALTIME 0
#define clockid_t int
#endif /// !defined(CLOCK_REALTIME)
#if !defined(CLOCK_HAS_GETTIME)
inline int clock_gettime(int id, timestruc_t* abstime) {
    return ::clock_relgettime(abstime);
}
#endif /// !defined(CLOCK_HAS_GETTIME)
#endif /// defined(SEMA_HAS_TIMEDWAIT)

namespace xos {
namespace mt {
namespace oracle {
namespace solaris {

///////////////////////////////////////////////////////////////////////
///  Class: SemaphoreT
///////////////////////////////////////////////////////////////////////
template 
<typename TError = int, TError VErrorSuccess = 0, TError VErrorFailed = -1,
 typename TAttached = sema_t*, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::SemaphoreT<TAttached, TUnattached, VUnattached>,
 class TSemaphore = extended::SemaphoreT
 <TError, VErrorSuccess, VErrorFailed, TAttached, TUnattached, VUnattached, TImplement>,
 class TImplements = TImplement, class TExtends = TSemaphore>

class _EXPORT_CLASS SemaphoreT: virtual public TImplements, public TExtends {
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
        if ((detached = CreateDetached(_sema, initiallyReleased))) {
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
#if defined(SEMA_HAS_TIMEDWAIT)
            if (((Attached)Unattached) != detached) {
                bool isLogged = ((this->IsLogged()) && (milliseconds >= this->TimedLoggedThreasholdMilliseconds()));
                int err = 0;
                timestruc_t untilTime;
    
                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "clock_gettime(CLOCK_REALTIME, &untilTime)...");
                if ((err = clock_gettime(CLOCK_REALTIME, &untilTime))) {
                    IS_ERR_LOGGED_ERROR("...failed " << err << " on clock_gettime(CLOCK_REALTIME, &untilTime)");
                    return LockFailed;
                } else {
                    IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...clock_gettime(CLOCK_REALTIME, &untilTime)");
                }
                untilTime.tv_sec +=  MSecondsSeconds(milliseconds);
                untilTime.tv_nsec +=  MSecondsNSeconds(MSecondsMSeconds(milliseconds));
    
                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "sema_timedwait(detached, &untilTime)...");
                if ((err = sema_timedwait(detached, &untilTime))) {
                    switch(err) {
                    case ETIME:
                        IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...ETIME err = "<< err << " on sema_timedwait(detached, &untilTime)");
                        return LockBusy;
                    case EINTR:
                        IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on sema_timedwait(detached, &untilTime)");
                        return LockInterrupted;
                    default:
                        IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on sema_timedwait(detached, &untilTime)");
                        return LockFailed;
                    }
                } else {
                    IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...sema_timedwait(detached, &untilTime)");
                    return LockSuccess;
                }
            }
#else /// defined(SEMA_HAS_TIMEDWAIT)
            IS_ERR_LOGGED_ERROR("...return AcquireInvalid");
            return AcquireInvalid;
#endif /// defined(SEMA_HAS_TIMEDWAIT)
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
            IS_ERR_LOGGED_TRACE("::sema_trywait(detached)...");
            if (!(err = ::sema_trywait(detached))) {
                IS_ERR_LOGGED_TRACE("...::sema_trywait(detached)");
                return AcquireSuccess;
            } else {
                if (EBUSY == (err)) {
                    IS_ERR_LOGGED_TRACE("...failed EBUSY err = " << err << " on ::sema_trywait(detached)");
                    return AcquireBusy;
                } else {
                    if (EINTR == (err)) {
                        IS_ERR_LOGGED_ERROR("...failed EINTR err = " << err << " on ::sema_trywait(detached)");
                        return AcquireInterrupted;
                    } else {
                        IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sema_trywait(detached)");
                    }
                }
            }
        }
        return AcquireFailed; 
    }
    virtual AcquireStatus UntimedAcquireDetached(Attached detached) const { 
        if (((Attached)Unattached) != detached) {
            int err = 0;
            IS_ERR_LOGGED_DEBUG("::sema_wait(detached)...");
            if (!(err = ::sema_wait(detached))) {
                IS_ERR_LOGGED_DEBUG("...::sema_wait(detached)");
                return AcquireSuccess;
            } else {
                if (EBUSY == (err)) {
                    IS_ERR_LOGGED_ERROR("...failed EBUSY err = " << err << " on ::sema_wait(detached)");
                    return AcquireBusy;
                } else {
                    IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sema_wait(detached)");
                }
            }
        }
        return AcquireFailed; 
    }
    virtual AcquireStatus UntimedReleaseDetached(Attached detached) const { 
        if (((Attached)Unattached) != detached) {
            int err = 0;
            IS_ERR_LOGGED_DEBUG("::sema_post(detached)...");
            if (!(err = ::sema_post(detached))) {
                IS_ERR_LOGGED_DEBUG("...::sema_post(detached)");
                return ReleaseSuccess;
            } else {
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sema_post(detached)");
            }
        }
        return ReleaseFailed; 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Attached CreateDetached(sema_t& sema) const {
        size_t initiallyReleased = 0;
        return CreateDetached(sema, initiallyReleased);
    }
    virtual Attached CreateDetached(sema_t& sema, size_t initiallyReleased) const {
        int type = USYNC_THREAD;
        return CreateDetached(sema, type, initiallyReleased);
    }
    virtual Attached CreateDetached(sema_t& sema, int type, size_t initiallyReleased) const {
        Attached detached = ((Attached)Unattached);
        void *arg = 0;
        int err = 0;
        IS_ERR_LOGGED_DEBUG("::sema_init(&sema, initiallyReleased, type, arg)...");
        if ((!(err = ::sema_init(&sema, initiallyReleased, type, arg)))) {
            IS_ERR_LOGGED_DEBUG("...::sema_init(&sema, initiallyReleased, type, arg)");
            return &sema;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sema_init(&sema, initiallyReleased, type, arg)");
        }
        return detached;
    }
    virtual bool DestroyDetached(Attached detached) const {
        if (((Attached)Unattached) != detached) {
            int err = 0;
            IS_ERR_LOGGED_DEBUG("::sema_destroy(detached)...");
            if (!(err = ::sema_destroy(detached))) {
                IS_ERR_LOGGED_DEBUG("...::sema_destroy(detached)");
                return true;
            } else {
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sema_destroy(detached)");
            }
        }
        return false;
    }
    
protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    inline int clock_gettime(int id, timestruc_t* abstime) const {
#if defined(SEMA_HAS_RELTIMEDWAIT)
        return ::clock_relgettime(abstime);
#else /// defined(SEMA_HAS_RELTIMEDWAIT)
        return ::clock_gettime(id, abstime);
#endif /// defined(SEMA_HAS_RELTIMEDWAIT)
    }   
    inline int sema_timedwait(sema_t *sp, timestruc_t *abstime) const {
#if defined(SEMA_HAS_RELTIMEDWAIT)
        return ::sema_reltimedwait(sp, abstime);
#else /// defined(SEMA_HAS_RELTIMEDWAIT)
        return ::sema_timedwait(sp, abstime);
#endif /// defined(SEMA_HAS_RELTIMEDWAIT)
    }   
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    sema_t _sema;
}; /// class _EXPORT_CLASS SemaphoreT
typedef SemaphoreT<> Semaphore;

} /// namespace solaris
} /// namespace oracle
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_ORACLE_SOLARIS_SEMAPHORE_HXX_
