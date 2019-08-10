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
///   Date: 7/21/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_ORACLE_SOLARIS_MUTEX_HXX_
#define _XOS_MT_ORACLE_SOLARIS_MUTEX_HXX_

#include "xos/mt/Mutex.hxx"

#if defined(SOLARIS)
#include <thread.h> 
#include <synch.h>
#else /// defined(SOLARIS)
#include "xos/platform/os/oracle/solaris/mutex.h"
#endif /// defined(SOLARIS)

#if !defined(MUTEX_HAS_TIMEDLOCK)
#define MUTEX_HAS_TIMEDLOCK
#endif /// !defined(MUTEX_HAS_TIMEDLOCK)

#if !defined(MUTEX_HAS_RELTIMEDLOCK)
#define MUTEX_HAS_RELTIMEDLOCK
#endif /// !defined(MUTEX_HAS_RELTIMEDLOCK)

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

#if defined(MUTEX_HAS_TIMEDLOCK)
#if !defined(CLOCK_REALTIME)
#define CLOCK_REALTIME 0
#define clockid_t int
#endif /// !defined(CLOCK_REALTIME)
#if !defined(CLOCK_HAS_GETTIME)
inline int clock_gettime(int id, timestruc_t* abstime) {
    return ::clock_relgettime(abstime);
}
#endif /// !defined(CLOCK_HAS_GETTIME)
#endif /// defined(MUTEX_HAS_TIMEDLOCK)

namespace xos {
namespace mt {
namespace oracle {
namespace solaris {

///////////////////////////////////////////////////////////////////////
///  Class: MutexT
///////////////////////////////////////////////////////////////////////
template 
<typename TError = int, TError VErrorSuccess = 0, TError VErrorFailed = -1,
 typename TAttached = mutex_t*, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::MutexT<TAttached, TUnattached, VUnattached>,
 class TMutex = extended::MutexT
 <TError, VErrorSuccess, VErrorFailed, TAttached, TUnattached, VUnattached, TImplement>,
 class TImplements = TImplement, class TExtends = TMutex>

class _EXPORT_CLASS MutexT: virtual public TImplements, public TExtends {
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
    virtual bool LockDetached(mutex_t& mutex) const { 
        if (LockSuccess != (UntimedLockDetached(mutex))) {
            return false; 
        }
        return true; 
    }
    virtual LockStatus TimedLockDetached(mutex_t& mutex, mseconds_t milliseconds) const { 
        if (0 < (milliseconds)) {
#if defined(MUTEX_HAS_TIMEDLOCK)
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

            IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "mutex_timedlock(&mutex, &untilTime)...");
            if ((err = mutex_timedlock(&mutex, &untilTime))) {
                switch(err) {
                case ETIME:
                    IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...ETIME err = "<< err << " on mutex_timedlock(&mutex, &untilTime)");
                    return LockBusy;
                case EINTR:
                    IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on mutex_timedlock(&mutex, &untilTime)");
                    return LockInterrupted;
                default:
                    IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on mutex_timedlock(&mutex, &untilTime)");
                    return LockFailed;
                }
            } else {
                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...mutex_timedlock(&mutex, &untilTime)");
                return LockSuccess;
            }
#else /// defined(MUTEX_HAS_TIMEDLOCK)
            IS_ERR_LOGGED_ERROR("...return LockInvalid");
            return LockInvalid;
#endif /// defined(MUTEX_HAS_TIMEDLOCK)
        } else {
            if (0 > (milliseconds)) {
                return UntimedLockDetached(mutex);
            } else {
                return TryLockDetached(mutex);
            }
        }
        return LockFailed; 
    }
    virtual LockStatus TryLockDetached(mutex_t& mutex) const { 
        int err = 0;
        IS_ERR_LOGGED_TRACE("::mutex_trylock(&mutex)...");
        if ((err = ::mutex_trylock(&mutex))) {
            switch(err) {
            case EBUSY:
                IS_ERR_LOGGED_TRACE("...EBUSY err = "<< err << " on ::mutex_trylock(&mutex)");
                return LockBusy;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::mutex_trylock(&mutex)");
                return LockFailed;
            }
        } else {
            IS_ERR_LOGGED_TRACE("...::mutex_trylock(&mutex)");
            return LockSuccess;
        }
        return LockFailed; 
    }
    virtual LockStatus UntimedLockDetached(mutex_t& mutex) const { 
        int err = 0;
        IS_LOGGED_DEBUG("::mutex_lock(&mutex)...");
        if ((err = ::mutex_lock(&mutex))) {
            switch(err) {
            case EBUSY:
                IS_ERR_LOGGED_ERROR("...EBUSY err = "<< err << " on ::mutex_lock(&mutex)");
                return LockBusy;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::mutex_lock(&mutex)");
                return LockFailed;
            }
        } else {
            IS_LOGGED_DEBUG("...::mutex_lock(&mutex)");
            return LockSuccess;
        }
        return LockFailed; 
    }
    virtual bool UnlockDetached(mutex_t& mutex) const { 
        int err = 0;
        IS_LOGGED_DEBUG("::mutex_unlock(&mutex)...");
        if (!(err = ::mutex_unlock(&mutex))) {
            IS_LOGGED_DEBUG("...::mutex_unlock(&mutex)");
            return true;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::mutex_unlock(&mutex)");
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
    virtual Attached CreateDetached(mutex_t& mutex) const {
        Attached detached = ((Attached)Unattached);
        int err = 0, type = USYNC_PROCESS | LOCK_ROBUST;
        void *arg = NULL;
        IS_ERR_LOGGED_DEBUG("::mutex_init(&mutex, type, arg)...");
        if (!(err = ::mutex_init(&mutex, type, arg))) {
            IS_ERR_LOGGED_DEBUG("...::mutex_init(&mutex, type, arg)");
            detached = &mutex;
        } else {
            IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pmutex_init(&mutex, type, arg)");
        }
        return detached;
    }
    virtual bool DestroyDetached(Attached detached) const {
        if (((Attached)Unattached) != detached) {
            int err = 0;
            IS_ERR_LOGGED_DEBUG("::mutex_destroy(detached)...");
            if (((err = ::mutex_destroy(detached)))) {
                IS_ERR_LOGGED_ERROR("...failed err = "<< err << " ::mutex_destroy(detached)");
            } else {
                IS_ERR_LOGGED_DEBUG("...::mutex_destroy(detached)");
                return true;
            }
        }
        return false;
    }

protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    inline int clock_gettime(int id, timestruc_t* abstime) const {
#if defined(MUTEX_HAS_RELTIMEDLOCK)
        return ::clock_relgettime(abstime);
#else /// defined(MUTEX_HAS_RELTIMEDLOCK)
        return ::clock_gettime(id, abstime);
#endif // defined(MUTEX_HAS_RELTIMEDLOCK)
    }
    inline int mutex_timedlock(mutex_t *mp, timestruc_t *abstime) const {
#if defined(MUTEX_HAS_RELTIMEDLOCK)
        return ::mutex_reltimedlock(mp, abstime);
#else /// defined(MUTEX_HAS_RELTIMEDLOCK)
        return ::mutex_timedlock(mp, abstime);
#endif // defined(MUTEX_HAS_RELTIMEDLOCK)
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    mutex_t _mutex;
}; /// class _EXPORT_CLASS MutexT
typedef MutexT<> Mutex;

} /// namespace solaris
} /// namespace oracle
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_ORACLE_SOLARIS_MUTEX_HXX_
