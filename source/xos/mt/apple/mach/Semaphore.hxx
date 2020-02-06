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
///   Date: 6/22/2019
///////////////////////////////////////////////////////////////////////
#if !defined(_XOS_MT_APPLE_MACH_SEMAPHORE_HXX_) || defined(_XOS_MT_APPLE_MACH_SEMAPHORE_CLASS_)
#if !defined(_XOS_MT_APPLE_MACH_SEMAPHORE_HXX_) && !defined(_XOS_MT_APPLE_MACH_SEMAPHORE_CLASS_)
#define _XOS_MT_APPLE_MACH_SEMAPHORE_HXX_
#endif /// !defined(_XOS_MT_APPLE_MACH_SEMAPHORE_HXX_) && !defined(_XOS_MT_APPLE_MACH_SEMAPHORE_CLASS_)

#if !defined(_XOS_MT_APPLE_MACH_SEMAPHORE_CLASS_)
#include "xos/mt/os/Os.hxx"
#include "xos/mt/Semaphore.hxx"

#include <mach/task.h>
#include <mach/mach.h>
#include <mach/semaphore.h>

#if !defined(SEMAPHORE_HAS_TRYWAIT)
inline kern_return_t semaphore_trywait(semaphore_t semaphore ) { 
    mach_timespec_t wait_time;    
    wait_time.tv_sec = 0;
    wait_time.tv_nsec = 0;
    return ::semaphore_timedwait(semaphore, wait_time); 
}
#define SEMAPHORE_HAS_TRYWAIT
#endif /// !defined(SEMAPHORE_HAS_TRYWAIT)

namespace xos {
namespace mt {
namespace apple {
namespace mach {
#endif /// !defined(_XOS_MT_APPLE_MACH_SEMAPHORE_CLASS_)

///////////////////////////////////////////////////////////////////////
///  Class: SemaphoreT
///////////////////////////////////////////////////////////////////////
template 
<typename TError = kern_return_t, TError VErrorSuccess = KERN_SUCCESS, TError VErrorFailed = KERN_FAILURE,
 typename TAttached = semaphore_t*, typename TUnattached = int, TUnattached VUnattached = 0,
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
    virtual AcquireStatus TryAcquire() { 
        return TryAcquireDetached(this->AttachedTo()); 
    }
    virtual AcquireStatus TimedAcquire(mseconds_t milliseconds) { 
        return TimedAcquireDetached(this->AttachedTo(), milliseconds); 
    }
    virtual AcquireStatus UntimedAcquire() { 
        return UntimedAcquireDetached(this->AttachedTo()); 
    }
    virtual AcquireStatus UntimedRelease() { 
        return UntimedReleaseDetached(this->AttachedTo()); 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual AcquireStatus TryAcquireDetached(Attached detached) const { 
#if defined(SEMAPHORE_HAS_TRYWAIT)
        if (((Attached)Unattached) != detached) {
            semaphore_t& semaphore = *detached;
            kern_return_t err = 0;

            IS_ERR_LOGGED_TRACE("::semaphore_trywait(semaphore)...");
            if (KERN_SUCCESS == (err = ::semaphore_trywait(semaphore))) {
                IS_ERR_LOGGED_TRACE("...::semaphore_trywait(semaphore)...");
                return AcquireSuccess;
            } else {
                if (KERN_OPERATION_TIMED_OUT == (err)) {
                    IS_ERR_LOGGED_TRACE("...failed KERN_OPERATION_TIMED_OUT err = " << err << " on ::semaphore_trywait(semaphore)");
                    return AcquireBusy;
                } else {
                    if (KERN_ABORTED == (err)) {
                        IS_ERR_LOGGED_ERROR("...failed KERN_ABORTED err = " << err << " on ::semaphore_trywait(semaphore)");
                        return AcquireInterrupted;
                    } else {
                        IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_trywait(semaphore)");
                    }
                }
            }
        }
        return AcquireFailed; 
#else /// defined(SEMAPHORE_HAS_TRYWAIT)
        IS_ERR_LOGGED_ERROR("...return AcquireInvalid");
        return AcquireInvalid; 
#endif /// defined(SEMAPHORE_HAS_TRYWAIT)
    }
    virtual AcquireStatus TimedAcquireDetached(Attached detached, mseconds_t milliseconds) const { 
        if (0 > (milliseconds)) {
            return UntimedAcquireDetached(detached);
        } else {
            if (((Attached)Unattached) != detached) {
                semaphore_t& semaphore = *detached;
                mseconds_t millisecondsThreashold = this->TimedLoggedThreasholdMilliseconds();
                bool isLogged = ((this->IsLogged()) && (milliseconds >= millisecondsThreashold));
                kern_return_t err = 0;
                mach_timespec_t wait_time;

                wait_time.tv_sec = MSecondsSeconds(milliseconds);
                wait_time.tv_nsec = MSecondsNSeconds(MSecondsMSeconds(milliseconds));

                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "::semaphore_timedwait(semaphore, wait_time)...");
                if (KERN_SUCCESS == (err = ::semaphore_timedwait(semaphore, wait_time))) {
                    IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...::semaphore_timedwait(semaphore, wait_time)...");
                    return AcquireSuccess;
                } else {
                    if (KERN_OPERATION_TIMED_OUT == (err)) {
                        IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...failed KERN_OPERATION_TIMED_OUT err = " << err << " on ::semaphore_timedwait(semaphore, wait_time)");
                        return AcquireBusy;
                    } else {
                        if (KERN_ABORTED == (err)) {
                            IS_ERR_LOGGED_ERROR("...failed KERN_ABORTED err = " << err << " on ::semaphore_timedwait(semaphore, wait_time)");
                            return AcquireInterrupted;
                        } else {
                            IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_timedwait(semaphore, wait_time)");
                        }
                    }
                }
            }
        }
        return AcquireFailed; 
    }
    virtual AcquireStatus UntimedAcquireDetached(Attached detached) const { 
        if (((Attached)Unattached) != detached) {
            semaphore_t& semaphore = *detached;
            kern_return_t err = 0;

            IS_ERR_LOGGED_DEBUG("::semaphore_wait(semaphore)...");
            if (KERN_SUCCESS == (err = ::semaphore_wait(semaphore))) {
                IS_ERR_LOGGED_DEBUG("...::semaphore_wait(semaphore)");
                return AcquireSuccess;
            } else {
                if (KERN_OPERATION_TIMED_OUT == (err)) {
                    IS_ERR_LOGGED_ERROR("...failed KERN_OPERATION_TIMED_OUT err = " << err << " on ::semaphore_wait(semaphore)");
                    return AcquireBusy;
                } else {
                    if (KERN_ABORTED == (err)) {
                        IS_ERR_LOGGED_ERROR("...failed KERN_ABORTED err = " << err << " on ::semaphore_wait(semaphore)");
                        return AcquireInterrupted;
                    } else {
                        IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_wait(semaphore)");
                    }
                }
            }
        }
        return AcquireFailed; 
    }
    virtual AcquireStatus UntimedReleaseDetached(Attached detached) const { 
        if (((Attached)Unattached) != detached) {
            semaphore_t& semaphore = *detached;
            kern_return_t err = 0;

            IS_ERR_LOGGED_DEBUG("::semaphore_signal(semaphore)...");
            if (KERN_SUCCESS == (err = ::semaphore_signal(semaphore))) {
                IS_ERR_LOGGED_DEBUG("...::semaphore_signal(semaphore)...");
                return ReleaseSuccess;
            } else {
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_signal(semaphore)");
            }
        }
        return ReleaseFailed; 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Attached CreateAttached() {
        return CreateAttached(0);
    }
    virtual Attached CreateAttached(size_t initiallyReleased) {
        Attached detached = ((Attached)Unattached);
        if ((detached = CreateDetached(_semaphore, initiallyReleased))) {
            this->Attach(detached);
        }
        return detached;
    }
    virtual Attached CreateDetached(semaphore_t& semaphore, size_t initiallyReleased) const {
        Attached detached = ((Attached)Unattached);
        sync_policy_t syncPolicy = SYNC_POLICY_FIFO;
        task_t task = mach_task_self();
        kern_return_t err = 0;

        IS_ERR_LOGGED_DEBUG("::semaphore_create(task, &semaphore, syncPolicy, initiallyReleased)...");
        if (KERN_SUCCESS != (err = ::semaphore_create(task, &semaphore, syncPolicy, initiallyReleased))) {
            IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_create(task, &semaphore, syncPolicy, initiallyReleased)");
        } else {
            IS_ERR_LOGGED_DEBUG("...::semaphore_create(task, &semaphore, syncPolicy, initiallyReleased)...");
            detached = &semaphore;
        }
        return detached;
    }
    virtual bool DestroyDetached(Attached detached) const {
        if (((Attached)Unattached) != detached) {
            semaphore_t& semaphore = *detached;
            task_t task = mach_task_self();
            kern_return_t err = 0;

            IS_ERR_LOGGED_DEBUG("::semaphore_destroy(task, semaphore)...")
            if (KERN_SUCCESS != (err = ::semaphore_destroy(task, semaphore))) {
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_destroy(task, semaphore)");
            } else {
                IS_ERR_LOGGED_DEBUG("...::semaphore_destroy(task, semaphore)")
                return true;
            }
        }
        return false;
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    semaphore_t _semaphore;
}; /// class _EXPORTED_ SemaphoreT
typedef SemaphoreT<> Semaphore;

#if !defined(_XOS_MT_APPLE_MACH_SEMAPHORE_CLASS_)
} /// namespace mach
} /// namespace apple
} /// namespace mt
} /// namespace xos
#endif /// !defined(_XOS_MT_APPLE_MACH_SEMAPHORE_CLASS_)

#endif /// !defined(_XOS_MT_APPLE_MACH_SEMAPHORE_HXX_) || defined(_XOS_MT_APPLE_MACH_SEMAPHORE_CLASS_)
