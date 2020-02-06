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
#ifndef _XOS_MT_SEMAPHORE_HXX_
#define _XOS_MT_SEMAPHORE_HXX_

#include "xos/mt/os/Sleep.hxx"
#include "xos/base/Acquired.hxx"
#include "xos/base/Logged.hxx"
#include "xos/base/Created.hxx"

#if !defined(XOS_MT_SEMAPHORE_CREATED)
#define XOS_MT_SEMAPHORE_CREATED() \
    IS_ERR_LOGGED_DEBUG("this->Created()..."); \
    if (!(this->Created())) { \
        IS_ERR_LOGGED_ERROR("...failed on this->Created() throw (CreateException(CreateFailed)..."); \
        throw (CreateException(CreateFailed)); \
    }
#endif /// !defined(XOS_MT_SEMAPHORE_CREATED)

#if !defined(XOS_MT_SEMAPHORE_DESTROYED)
#define XOS_MT_SEMAPHORE_DESTROYED() \
    IS_ERR_LOGGED_DEBUG("this->Destroyed()..."); \
    if (!(this->Destroyed())) {  \
        IS_ERR_LOGGED_ERROR("...failed on this->Destroyed() throw (CreateException(DestroyFailed))..."); \
        throw (CreateException(DestroyFailed)); \
    }
#endif /// !defined(XOS_MT_SEMAPHORE_DESTROYED)

namespace xos {
namespace mt {

///////////////////////////////////////////////////////////////////////
///  Class: SemaphoreT
///////////////////////////////////////////////////////////////////////
template 
<class TAcquired = Released,
 class TLogged = LoggedT<TAcquired>,
 class TImplements = CreatorT<TLogged> >

class _EXPORTED_ SemaphoreT: virtual public TImplements {
public:
    typedef TImplements Implements;
}; /// class _EXPORTED_ SemaphoreT
typedef SemaphoreT<> Semaphore;

namespace implemented {
///////////////////////////////////////////////////////////////////////
///  Class: SemaphoreT
///////////////////////////////////////////////////////////////////////
template 
<typename TAttached = pointer_t,
 typename TUnattached = int, TUnattached VUnattached = 0,
 class TAttacher = AttacherT<TAttached, TUnattached, VUnattached, mt::Semaphore>,
 class TImplements = TAttacher>

class _EXPORTED_ SemaphoreT: virtual public TImplements {
public:
    typedef TImplements Implements;

    virtual bool Acquire() { 
        if (AcquireSuccess == (this->UntimedAcquire())) {
            return true;
        }
        return false; 
    }
    virtual bool Release() { 
        if (ReleaseSuccess == (this->UntimedRelease())) {
            return true;
        }
        return false; 
    }
}; /// class _EXPORTED_ SemaphoreT
typedef SemaphoreT<> Semaphore;
} /// namespace implemented

namespace extended {
///////////////////////////////////////////////////////////////////////
///  Class: SemaphoreT
///////////////////////////////////////////////////////////////////////
template 
<typename TError = int, TError VErrorSuccess = 0, TError VErrorFailed = 1,
 typename TAttached = pointer_t, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::SemaphoreT<TAttached, TUnattached, VUnattached>,
 class TLogged = xos::extended::LoggedT<TImplement, Base>,
 class TAttachedT = AttachedT<TAttached, TUnattached, VUnattached, TImplement, TLogged>,
 class TCreated = CreatedT<TAttached, TUnattached, VUnattached, TImplement, TAttachedT>,
 class TImplements = TImplement, class TExtends = TCreated>

class _EXPORTED_ SemaphoreT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef TError Error;
    static const TError ErrorSuccess = VErrorSuccess;
    static const TError ErrorFailed = VErrorFailed;

    typedef TAttached Attached;
    typedef TUnattached UnattachedT;
    static const TUnattached Unattached = VUnattached;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    SemaphoreT(Attached detached, bool isCreated, bool isLogged, bool isErrLogged): Extends(detached, isCreated) {
        this->SetIsLogged(isLogged);
        this->SetIsErrLogged(isErrLogged);
    }
    SemaphoreT(Attached detached, bool isCreated, bool isLogged): Extends(detached, isCreated) {
        this->SetIsLogged(isLogged);
        this->SetIsErrLogged(isLogged);
    }
    SemaphoreT(Attached detached, bool isCreated): Extends(detached, isCreated) {
    }
    SemaphoreT(Attached detached): Extends(detached) {
    }
    SemaphoreT(bool isLogged, bool isErrLogged) {
        this->SetIsLogged(isLogged);
        this->SetIsErrLogged(isErrLogged);
    }
    SemaphoreT(bool isLogged) {
        this->SetIsLogged(isLogged);
        this->SetIsErrLogged(isLogged);
    }
    SemaphoreT(const SemaphoreT &copy): Extends(copy.AttachedTo(), false) {
    }
    SemaphoreT() {
    }
    virtual ~SemaphoreT() {
        XOS_MT_SEMAPHORE_DESTROYED();
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    using Extends::Create;
    virtual bool Create(size_t initiallyReleased) {
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = CreateAttached(initiallyReleased))) {
            this->SetIsCreated();
            return true;
        }
        return false;
    }
    using Extends::CreateAttached;
    virtual Attached CreateAttached(size_t initiallyReleased) {
        Attached detached = ((Attached)Unattached);
        if (!(initiallyReleased)) {
            detached = CreateAttached();
        } else {
            if ((detached = CreateDetached(initiallyReleased))) {
                this->Attach(detached);
            }
        }
        return detached;
    }
    using Extends::CreateDetached;
    virtual Attached CreateDetached(size_t initiallyReleased) const {
        Attached detached = ((Attached)Unattached);
        if (!(initiallyReleased)) {
            detached = CreateDetached();
        }
        return detached;
    }
}; /// class _EXPORTED_ SemaphoreT
typedef SemaphoreT<> Semaphore;
} /// namespace extended

namespace derived {
///////////////////////////////////////////////////////////////////////
///  Class: SemaphoreT
///////////////////////////////////////////////////////////////////////
template 
<typename TError = int, TError VErrorSuccess = 0, TError VErrorFailed = 1,
 typename TAttached = pointer_t, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::SemaphoreT<TAttached, TUnattached, VUnattached>,
 class TSemaphore = extended::SemaphoreT
 <TError, VErrorSuccess, VErrorFailed, TAttached, TUnattached, VUnattached, TImplement>,
 class TImplements = TImplement, class TExtends = TSemaphore>

class _EXPORTED_ SemaphoreT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Extends::Error Error;
    static const Error ErrorSuccess = Extends::ErrorSuccess;
    static const Error ErrorFailed = Extends::ErrorFailed;

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
    virtual AcquireStatus TimedAcquire(mseconds_t milliseconds) { 
        return TimedAcquireDetached(this->AttachedTo(), milliseconds); 
    }
    virtual AcquireStatus TimedAcquireDetached(Attached detached, mseconds_t milliseconds) const { 
        if (((Attached)Unattached) != detached) {
            if (0 < (milliseconds)) {
                mt::os::Sleep sleep(milliseconds);
                return AcquireBusy;
            }
        }
        return AcquireFailed; 
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    using Extends::CreateDetached;
    virtual Attached CreateDetached() const {
        Attached detached = ((Attached)Unattached);
        IS_ERR_LOGGED_DEBUG("CreateSemaphore()...");
        if (((Attached)Unattached) != (detached = CreateSemaphore())) {
            IS_ERR_LOGGED_DEBUG("...CreateSemaphore()");
        } else {
            Error error = ErrorFailed;
            IS_ERR_LOGGED_ERROR("...failed error = "<< error << " on CreateSemaphore()");
        }
        return detached;
    }
    virtual bool DestroyDetached(Attached detached) const {
        if (((Attached)Unattached) != detached) {
            Error error = ErrorFailed;
            IS_ERR_LOGGED_DEBUG("DestroySemaphore(detached)...");
            if ((ErrorSuccess != (error = DestroySemaphore(detached)))) {
                IS_ERR_LOGGED_ERROR("...failed error = "<< error << " on DestroySemaphore()");
            } else {
                IS_ERR_LOGGED_DEBUG("...DestroySemaphore(detached)");
                return true;
            }
        }
        return false;
    }

protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Attached CreateSemaphore() const {
        return (Attached)this;
    }
    virtual Error DestroySemaphore(Attached detached) const {
        Error error = (this != detached)?ErrorFailed:ErrorSuccess;
        return error;
    }
}; /// class _EXPORTED_ SemaphoreT
typedef SemaphoreT<> Semaphore;
} /// namespace derived

} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_SEMAPHORE_HXX_
