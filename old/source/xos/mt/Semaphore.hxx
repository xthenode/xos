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
///   Date: 5/8/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_SEMAPHORE_HXX_
#define _XOS_MT_SEMAPHORE_HXX_

#include "xos/base/Acquired.hxx"
#include "xos/base/logged.hxx"
#include "xos/base/Created.hxx"

#if !defined(XOS_MT_SEMAPHORE_CREATED)
#define XOS_MT_SEMAPHORE_CREATED() \
    IS_ERR_LOGGED_DEBUG("this->Created()..."); \
    if (!(this->Created())) { \
        const CreateException e(CreateFailed); \
        IS_ERR_LOGGED_ERROR("...failed on this->Created() throw (const CreateException e = CreateFailed)..."); \
        throw (e); \
    }
#endif /// !defined(XOS_MT_SEMAPHORE_CREATED)

#if !defined(XOS_MT_SEMAPHORE_DESTROYED)
#define XOS_MT_SEMAPHORE_DESTROYED() \
    IS_ERR_LOGGED_DEBUG("this->Destroyed()..."); \
    if (!(this->Destroyed())) {  \
        const CreateException e(DestroyFailed); \
        IS_ERR_LOGGED_ERROR("...failed on this->Destroyed() throw (const CreateException e(DestroyFailed))..."); \
        throw (e); \
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

class _EXPORT_CLASS SemaphoreT: virtual public TImplements {
public:
    typedef TImplements Implements;
};
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

class _EXPORT_CLASS SemaphoreT: virtual public TImplements {
public:
    typedef TImplements Implements;    
    typedef TAttached Attached;
    typedef TUnattached UnattachedT;
    static const TUnattached Unattached = VUnattached;

    virtual bool Acquire() { 
        if (AcquireSuccess == (this->UntimedAcquire())) {
            return true;
        }
        return false; 
    }
    virtual AcquireStatus TryAcquire() { 
        return this->TimedAcquire(0); 
    }
    virtual bool Release() { 
        if (ReleaseSuccess == (this->UntimedRelease())) {
            return true;
        }
        return false; 
    }
    virtual AcquireStatus TryRelease() { 
        return this->TimedRelease(0); 
    }
};
typedef SemaphoreT<> Semaphore;
} /// namespace implemented

namespace extended {
///////////////////////////////////////////////////////////////////////
///  Class: SemaphoreT
///////////////////////////////////////////////////////////////////////
template 
<typename TError = int, TError VErrorSuccess = 0,
 typename TAttached = pointer_t, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::SemaphoreT<TAttached, TUnattached, VUnattached>,
 class TLogged = xos::extended::LoggedT<TImplement, Base>,
 class TAttachedT = AttachedT<TAttached, TUnattached, VUnattached, TImplement, TLogged>,
 class TCreated = CreatedT<TAttached, TUnattached, VUnattached, TImplement, TAttachedT>,
 class TImplements = TImplement, class TExtends = TCreated>

class _EXPORT_CLASS SemaphoreT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef TError error_t;
    static const TError ErrorSuccess = VErrorSuccess;

    typedef TAttached Attached;
    typedef TUnattached UnattachedT;
    static const TUnattached Unattached = VUnattached;

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

    using Extends::Create;
    virtual bool Create(size_t initiallyReleased) {
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = CreateAttached(initiallyReleased))) {
            this->SetIsCreated();
            return true;
        }
        return false;
    }
    virtual Attached CreateAttached() {
        return CreateAttached(0);
    }
    virtual Attached CreateAttached(size_t initiallyReleased) {
        Attached detached = ((Attached)Unattached);
        if ((detached = CreateDetached(initiallyReleased))) {
            this->Attach(detached);
        }
        return detached;
    }
    virtual Attached CreateDetached() const {
        return CreateDetached(0);
    }
    virtual Attached CreateDetached(size_t initiallyReleased) const {
        Attached detached = ((Attached)Unattached);
        return detached;
    }
};
typedef SemaphoreT<> Semaphore;
} /// namespace extended

inline pointer_t CreateSemaphore(size_t initiallyReleased) {
    extended::Semaphore* semaphore = 0;
    if ((semaphore = new extended::Semaphore(false, false))) {
        return semaphore;
    }
    return 0;
}
inline pointer_t CreateSemaphore() {
    extended::Semaphore* semaphore = 0;
    if ((semaphore = new extended::Semaphore(false, false))) {
        return semaphore;
    }
    return 0;
}
inline int DestroySemaphore(pointer_t pointer) {
    extended::Semaphore* semaphore = 0;
    if ((semaphore = ((extended::Semaphore*)pointer))) {
        delete semaphore;
        return 0;
    }
    return 1;
}

namespace derived {
typedef int SemaphoreTError;
static const SemaphoreTError SemaphoreVErrorSuccess = 0;
typedef pointer_t SemaphoreTAttached;
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

    virtual Attached CreateDetached(size_t initiallyReleased) const {
        Attached detached = ((Attached)Unattached);
        IS_ERR_LOGGED_DEBUG("CreateSemaphore()...");
        if (((Attached)Unattached) != (detached = CreateSemaphore(initiallyReleased))) {
            IS_ERR_LOGGED_DEBUG("...CreateSemaphore()");
        } else {
            error_t error = 1;
            IS_ERR_LOGGED_ERROR("...failed error = "<< error << " on CreateSemaphore()");
        }
        return detached;
    }
    virtual Attached CreateDetached() const {
        Attached detached = ((Attached)Unattached);
        IS_ERR_LOGGED_DEBUG("CreateSemaphore()...");
        if (((Attached)Unattached) != (detached = CreateSemaphore())) {
            IS_ERR_LOGGED_DEBUG("...CreateSemaphore()");
        } else {
            error_t error = 1;
            IS_ERR_LOGGED_ERROR("...failed error = "<< error << " on CreateSemaphore()");
        }
        return detached;
    }
    virtual bool DestroyDetached(Attached detached) const {
        if (((Attached)Unattached) != detached) {
            error_t error = 1;
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
};
typedef SemaphoreT<> Semaphore;
} /// namespace derived

} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_SEMAPHORE_HXX_
