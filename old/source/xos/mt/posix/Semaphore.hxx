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
#ifndef _XOS_MT_POSIX_SEMAPHORE_HXX_
#define _XOS_MT_POSIX_SEMAPHORE_HXX_

#include "xos/mt/os/Os.hxx"
#include "xos/mt/Semaphore.hxx"

#include <semaphore.h>
#include <errno.h>

#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#define SEM_HAS_TIMEDWAIT
#endif // defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )

#define SEMAPHORE_PROCESS_PRIVATE 0
#define SEMAPHORE_PROCESS_SHARED  1

namespace xos {
namespace mt {
namespace posix {

typedef int SemaphoreTError;
static const SemaphoreTError SemaphoreVErrorSuccess = 0;
typedef sem_t* SemaphoreTAttached;
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

    typedef typename Extends::Attached Attached;
    static const typename Extends::UnattachedT Unattached = Extends::Unattached;
    
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

    using Extends::CreateAttached;
    virtual Attached CreateAttached(size_t initiallyReleased) {
        Attached detached = ((Attached)Unattached);
        if ((detached = CreateDetached(_sem, initiallyReleased))) {
            this->Attach(detached);
        }
        return detached;
    }
    virtual Attached CreateDetached(sem_t& sem, size_t initiallyReleased) const {
        Attached detached = ((Attached)Unattached);
        int pshared = SEMAPHORE_PROCESS_PRIVATE;
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
            sem_t* sem = detached;
            int err = 0;
            IS_ERR_LOGGED_DEBUG("::sem_destroy(sem)...");
            if (!(err = ::sem_destroy(sem))) {
                IS_ERR_LOGGED_DEBUG("...::sem_destroy(sem)");
                return true;
            } else {
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sem_destroy(sem)");
            }
        }
        return false;
    }

    virtual AcquireStatus TimedAcquire(mseconds_t milliseconds) { 
        if (0 > (milliseconds)) {
            return UntimedAcquire();
        } else {
#if defined(SEM_HAS_TIMEDWAIT)
            sem_t* sem = 0;

            if ((sem = this->AttachedTo())) {
                int err = 0;
                struct timespec untilTime;

                clock_gettime(CLOCK_REALTIME, &untilTime);
                untilTime.tv_sec +=  MSecondsSeconds(milliseconds);
                untilTime.tv_nsec +=  MSecondsNSeconds(MSecondsMSeconds(milliseconds));

                IS_ERR_LOGGED_TRACE("::sem_timedwait(sem, &untilTime)...");
                if (!(err = ::sem_timedwait(sem, &untilTime))) {
                    IS_ERR_LOGGED_TRACE("...::sem_timedwait(sem, &untilTime)...");
                    return AcquireSuccess;
                } else {
                    if (ETIMEDOUT == (err)) {
                        IS_ERR_LOGGED_TRACE("...failed ETIMEDOUT err = " << err << " on ::sem_timedwait(sem, &untilTime)");
                        return AcquireBusy;
                    } else {
                        if (EINTR == (err)) {
                            IS_ERR_LOGGED_ERROR("...failed EINTR err = " << err << " on ::sem_timedwait(sem, &untilTime)");
                            return AcquireInterrupted;
                        } else {
                            IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sem_timedwait(sem, &untilTime)");
                        }
                    }
                }
            }
#else /// defined(SEM_HAS_TIMEDWAIT)
            IS_ERR_LOGGED_ERROR("...return AcquireInvalid");
            return AcquireInvalid;
#endif /// defined(SEM_HAS_TIMEDWAIT)
        }
        return AcquireFailed; 
    }
    virtual AcquireStatus UntimedAcquire() { 
        sem_t* sem = 0;

        if ((sem = this->AttachedTo())) {
            int err = 0;

            IS_ERR_LOGGED_DEBUG("::sem_wait(sem)...");
            if (!(err = ::sem_wait(sem))) {
                IS_ERR_LOGGED_DEBUG("...::sem_wait(sem)");
                return AcquireSuccess;
            } else {
                if (ETIMEDOUT == (err)) {
                    IS_ERR_LOGGED_ERROR("...failed ETIMEDOUT err = " << err << " on ::sem_wait(sem)");
                    return AcquireBusy;
                } else {
                    if (EINTR == (err)) {
                        IS_ERR_LOGGED_ERROR("...failed EINTR err = " << err << " on ::sem_wait(sem)");
                        return AcquireInterrupted;
                    } else {
                        IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::sem_wait(sem)");
                    }
                }
            }
        }
        return AcquireFailed; 
    }
    virtual AcquireStatus UntimedRelease() { 
        sem_t* sem = 0;

        if ((sem = this->AttachedTo())) {
            int err = 0;

            IS_ERR_LOGGED_DEBUG("::sem_post(sem)...");
            if (!(err = ::sem_post(sem))) {
                IS_ERR_LOGGED_DEBUG("...::sem_post(sem)");
                return ReleaseSuccess;
            } else {
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::semaphore_signal(*semaphore)");
            }
        }
        return ReleaseFailed; 
    }

protected:
    sem_t _sem;
};
typedef SemaphoreT<> Semaphore;

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_POSIX_SEMAPHORE_HXX_
