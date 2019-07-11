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
///   Date: 5/9/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_APPLE_MACH_SEMAPHORE_HXX_
#define _XOS_MT_APPLE_MACH_SEMAPHORE_HXX_

#include <mach/task.h>
#include <mach/mach.h>
#include <mach/semaphore.h>

#include "xos/mt/Semaphore.hxx"

namespace xos {
namespace mt {
namespace apple {
namespace mach {

typedef int SemaphoreTError;
static const SemaphoreTError SemaphoreVErrorSuccess = 0;
typedef semaphore_t* SemaphoreTAttached;
typedef int SemaphoreTUnattached;
static const SemaphoreTUnattached SemaphoreVUnattached = 0;
///////////////////////////////////////////////////////////////////////
///  Class: SemaphoreT
///////////////////////////////////////////////////////////////////////
template 
<class TImplement = implemented::SemaphoreT
 <SemaphoreTAttached, SemaphoreTUnattached, SemaphoreVUnattached>,
 class TSemaphore = extended::SemaphoreT
 <SemaphoreTError, SemaphoreVErrorSuccess, 
  SemaphoreTAttached, SemaphoreTUnattached, SemaphoreVUnattached, TImplement>,
 class TImplements = TImplement, class TExtends = TSemaphore>

class _EXPORT_CLASS SemaphoreT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Extends::error_t error_t;
    static const error_t ErrorSuccess = Extends::ErrorSuccess;

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
    using Extends::CreateAttached;
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
        int err = 0;

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
            semaphore_t* semaphore = detached;
            task_t task = mach_task_self();
            int err = 0;

            IS_ERR_LOGGED_DEBUG("::semaphore_destroy(task, *semaphore)...")
            if (KERN_SUCCESS != (err = ::semaphore_destroy(task, *semaphore))) {
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_destroy(task, *semaphore)");
            } else {
                IS_ERR_LOGGED_DEBUG("...::semaphore_destroy(task, *semaphore)")
                return true;
            }
        }
        return false;
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual AcquireStatus TryAcquire() { 
        return TimedAcquire(0); 
    }
    virtual AcquireStatus TimedAcquire(mseconds_t milliseconds) { 
        if (0 > (milliseconds)) {
            return UntimedAcquire();
        } else {
            semaphore_t* semaphore = 0;

            if ((semaphore = this->AttachedTo())) {
                mseconds_t millisecondsThreashold = this->TimedLoggedThreasholdMilliseconds();
                bool isLogged = (milliseconds >= millisecondsThreashold);
                int err = 0;
                mach_timespec_t wait_time;

                wait_time.tv_sec = MSecondsSeconds(milliseconds);
                wait_time.tv_nsec = MSecondsNSeconds(MSecondsMSeconds(milliseconds));

                IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "::semaphore_timedwait(*semaphore, wait_time)...");
                if (KERN_SUCCESS == (err = ::semaphore_timedwait(*semaphore, wait_time))) {
                    IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "...::semaphore_timedwait(*semaphore, wait_time)...");
                    return AcquireSuccess;
                } else {
                    if (KERN_OPERATION_TIMED_OUT == (err)) {
                        IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "...failed KERN_OPERATION_TIMED_OUT err = " << err << " on ::semaphore_timedwait(*semaphore, wait_time)");
                        return AcquireBusy;
                    } else {
                        if (KERN_ABORTED == (err)) {
                            IS_ERR_LOGGED_ERROR("...failed KERN_ABORTED err = " << err << " on ::semaphore_timedwait(*semaphore, wait_time)");
                            return AcquireInterrupted;
                        } else {
                            IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_timedwait(*semaphore, wait_time)");
                        }
                    }
                }
            }
        }
        return AcquireFailed; 
    }
    virtual AcquireStatus UntimedAcquire() { 
        semaphore_t* semaphore = 0;

        if ((semaphore = this->AttachedTo())) {
            int err = 0;

            IS_ERR_LOGGED_DEBUG("::semaphore_wait(*semaphore)...");
            if (KERN_SUCCESS == (err = ::semaphore_wait(*semaphore))) {
                IS_ERR_LOGGED_DEBUG("...::semaphore_wait(*semaphore)");
                return AcquireSuccess;
            } else {
                if (KERN_OPERATION_TIMED_OUT == (err)) {
                    IS_ERR_LOGGED_ERROR("...failed KERN_OPERATION_TIMED_OUT err = " << err << " on ::semaphore_wait(*semaphore)");
                    return AcquireBusy;
                } else {
                    if (KERN_ABORTED == (err)) {
                        IS_ERR_LOGGED_ERROR("...failed KERN_ABORTED err = " << err << " on ::semaphore_wait(*semaphore)");
                        return AcquireInterrupted;
                    } else {
                        IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_wait(*semaphore)");
                    }
                }
            }
        }
        return AcquireFailed; 
    }
    virtual AcquireStatus UntimedRelease() { 
        semaphore_t* semaphore = 0;

        if ((semaphore = this->AttachedTo())) {
            int err = 0;

            IS_ERR_LOGGED_DEBUG("::semaphore_signal(*semaphore)...");
            if (KERN_SUCCESS == (err = ::semaphore_signal(*semaphore))) {
                IS_ERR_LOGGED_DEBUG("...::semaphore_signal(*semaphore)...");
                return ReleaseSuccess;
            } else {
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_signal(*semaphore)");
            }
        }
        return ReleaseFailed; 
    }

protected:
    semaphore_t _semaphore;
}; /// class _EXPORT_CLASS SemaphoreT

typedef SemaphoreT<> Semaphore;

} /// namespace mach
} /// namespace apple
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_APPLE_MACH_SEMAPHORE_HXX_
