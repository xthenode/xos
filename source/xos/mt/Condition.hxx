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
///   Date: 6/10/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_CONDITION_HXX_
#define _XOS_MT_CONDITION_HXX_

#include "xos/base/Signaled.hxx"
#include "xos/base/logged.hxx"
#include "xos/base/Created.hxx"
#include "xos/mt/Mutex.hxx"

#if !defined(XOS_MT_CONDITION_CREATED)
#define XOS_MT_CONDITION_CREATED() \
    IS_ERR_LOGGED_DEBUG("this->Created()..."); \
    if (!(this->Created())) { \
        IS_ERR_LOGGED_ERROR("...failed on this->Created() throw (CreateException(CreateFailed)..."); \
        throw (CreateException(CreateFailed)); \
    } else { \
        IS_ERR_LOGGED_DEBUG("...this->Created()"); \
    }
#endif /// !defined(XOS_MT_CONDITION_CREATED)

#if !defined(XOS_MT_CONDITION_DESTROYED)
#define XOS_MT_CONDITION_DESTROYED() \
    IS_ERR_LOGGED_DEBUG("this->Destroyed()..."); \
    if (!(this->Destroyed())) {  \
        IS_ERR_LOGGED_ERROR("...failed on this->Destroyed() throw (CreateException(DestroyFailed))..."); \
        throw (CreateException(DestroyFailed)); \
    } else { \
        IS_ERR_LOGGED_DEBUG("...this->Destroyed()"); \
    }
#endif /// !defined(XOS_MT_CONDITION_DESTROYED)

namespace xos {
namespace mt {

///////////////////////////////////////////////////////////////////////
///  Class: ConditionT
///////////////////////////////////////////////////////////////////////
template 
<class TSignaled = Signaled,
 class TLogged = LoggedT<TSignaled>,
 class TImplements = CreatorT<TLogged> >

class _EXPORT_CLASS ConditionT: virtual public TImplements {
public:
    typedef TImplements Implements;
}; /// class _EXPORT_CLASS ConditionT
typedef ConditionT<> Condition;

namespace implemented {
///////////////////////////////////////////////////////////////////////
///  Class: ConditionT
///////////////////////////////////////////////////////////////////////
template 
<typename TAttached = pointer_t,
 typename TUnattached = int, TUnattached VUnattached = 0,
 class TAttacher = AttacherT<TAttached, TUnattached, VUnattached, mt::Condition>,
 class TImplements = TAttacher>

class _EXPORT_CLASS ConditionT: virtual public TImplements {
public:
    typedef TImplements Implements;
}; /// class _EXPORT_CLASS ConditionT
typedef ConditionT<> Condition;
} /// namespace implemented

namespace extended {
///////////////////////////////////////////////////////////////////////
///  Class: ConditionT
///////////////////////////////////////////////////////////////////////
template 
<typename TError = int, TError VErrorSuccess = 0,
 typename TAttached = pointer_t, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::ConditionT<TAttached, TUnattached, VUnattached>,
 class TLogged = xos::extended::LoggedT<TImplement, Base>,
 class TAttachedT = AttachedT<TAttached, TUnattached, VUnattached, TImplement, TLogged>,
 class TCreated = CreatedT<TAttached, TUnattached, VUnattached, TImplement, TAttachedT>,
 class TImplements = TImplement, class TExtends = TCreated>

class _EXPORT_CLASS ConditionT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef TError Error;
    static const TError ErrorSuccess = VErrorSuccess;

    typedef TAttached Attached;
    typedef TUnattached UnattachedT;
    static const TUnattached Unattached = VUnattached;

    ConditionT(Attached detached, bool isCreated, bool isLogged, bool isErrLogged): Extends(detached, isCreated) {
        this->SetIsLogged(isLogged);
        this->SetIsErrLogged(isErrLogged);
    }
    ConditionT(Attached detached, bool isCreated, bool isLogged): Extends(detached, isCreated) {
        this->SetIsLogged(isLogged);
        this->SetIsErrLogged(isLogged);
    }
    ConditionT(Attached detached, bool isCreated): Extends(detached, isCreated) {
    }
    ConditionT(Attached detached): Extends(detached) {
    }
    ConditionT(bool isLogged, bool isErrLogged) {
        this->SetIsLogged(isLogged);
        this->SetIsErrLogged(isErrLogged);
    }
    ConditionT(bool isLogged) {
        this->SetIsLogged(isLogged);
        this->SetIsErrLogged(isLogged);
    }
    ConditionT(const ConditionT& copy): Extends(copy.AttachedTo(), false) {
    }
    ConditionT() {
    }
    virtual ~ConditionT() {
        XOS_MT_CONDITION_DESTROYED();
    }

    using Extends::Create;
    virtual bool Create(bool initiallySignaled) {
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = CreateAttached(initiallySignaled))) {
            this->SetIsCreated();
            return true;
        }
        return false;
    }
    using Extends::CreateAttached;
    virtual Attached CreateAttached(bool initiallySignaled) {
        Attached detached = ((Attached)Unattached);
        if (!(initiallySignaled)) {
            detached = CreateAttached();
        } else {
            if ((detached = CreateDetached(initiallySignaled))) {
                this->Attach(detached);
            }
        }
        return detached;
    }
    using Extends::CreateDetached;
    virtual Attached CreateDetached(bool initiallySignaled) const {
        Attached detached = ((Attached)Unattached);
        if (!(initiallySignaled)) {
            detached = CreateDetached();
        }
        return detached;
    }
}; /// class _EXPORT_CLASS ConditionT
typedef ConditionT<> Condition;
} /// namespace extended

namespace derived {
typedef int ConditionError;
static const ConditionError ConditionErrorSuccess = 0;
typedef pointer_t ConditionAttached;
typedef int ConditionUnattachedT;
static const ConditionUnattachedT ConditionUnattached = 0;
///////////////////////////////////////////////////////////////////////
///  Class: ConditionT
///////////////////////////////////////////////////////////////////////
template 
<class TImplement = implemented::ConditionT
 <ConditionAttached, ConditionUnattachedT, ConditionUnattached>,
 class TCondition = extended::ConditionT
 <ConditionError, ConditionErrorSuccess, 
  ConditionAttached, ConditionUnattachedT, ConditionUnattached, TImplement>,
 class TImplements = TImplement, class TExtends = TCondition>

class _EXPORT_CLASS ConditionT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Extends::Error Error;
    static const Error ErrorSuccess = Extends::ErrorSuccess;

    typedef typename Extends::Attached Attached;
    static const typename Extends::UnattachedT Unattached = Extends::Unattached;

    ConditionT(Attached detached, bool isCreated, bool isLogged, bool isErrLogged): Extends(detached, isCreated, isLogged, isErrLogged) {
    }
    ConditionT(Attached detached, bool isCreated, bool isLogged): Extends(detached, isCreated, isLogged) {
    }
    ConditionT(Attached detached, bool isCreated): Extends(detached, isCreated) {
    }
    ConditionT(Attached detached): Extends(detached) {
    }
    ConditionT(bool isLogged, bool isErrLogged): Extends(isLogged, isErrLogged) {
        XOS_MT_CONDITION_CREATED();
    }
    ConditionT(bool isLogged): Extends(isLogged) {
        XOS_MT_CONDITION_CREATED();
    }
    ConditionT(const ConditionT& copy): Extends(copy) {
    }
    ConditionT(Mutex& mutex, bool& signaled) {
        XOS_MT_CONDITION_CREATED();
    }
    ConditionT() {
        XOS_MT_CONDITION_CREATED();
    }
    virtual ~ConditionT() {
        XOS_MT_CONDITION_DESTROYED();
    }

    virtual Attached CreateDetached(bool initiallySignaled) const {
        Attached detached = ((Attached)Unattached);
        IS_ERR_LOGGED_DEBUG("CreateCondition(initiallySignaled)...");
        if (((Attached)Unattached) != (detached)) {
        } else {
            Error error = 1;
            IS_ERR_LOGGED_ERROR("...failed error = "<< error << " on CreateCondition(initiallySignaled)");
        }
        return detached;
    }
    virtual Attached CreateDetached() const {
        Attached detached = ((Attached)Unattached);
        IS_ERR_LOGGED_DEBUG("CreateCondition(false)...");
        if (((Attached)Unattached) != (detached)) {
        } else {
            Error error = 1;
            IS_ERR_LOGGED_ERROR("...failed error = "<< error << " on CreateCondition(false)");
        }
        return detached;
    }
    virtual bool DestroyDetached(Attached detached) const {
        if (((Attached)Unattached) != detached) {
            Error error = 1;
            IS_ERR_LOGGED_DEBUG("DestroyCondition(detached)...");
            if ((ErrorSuccess != (error))) {
                IS_ERR_LOGGED_ERROR("...failed error = "<< error << " on DestroyCondition()");
            } else {
                return true;
            }
        }
        return false;
    }
}; /// class _EXPORT_CLASS ConditionT
typedef ConditionT<> Condition;
} /// namespace derived

} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_CONDITION_HXX_
