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
///   File: Created.hxx
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_BASE_CREATED_HXX_
#define _XOS_BASE_CREATED_HXX_

#include "xos/base/Attached.hxx"

namespace xos {

///////////////////////////////////////////////////////////////////////
/// Enum: CreateStatus
///////////////////////////////////////////////////////////////////////
typedef int CreateStatus;
enum {
    DestroySucces,
    CreateSuccess = DestroySucces,
    CreateFailed,
    DestroyFailed,

    CreateBusy,
    CreateInterrupted,
    CreateInvalid,

    DestroyBusy,
    DestroyInterrupted,
    DestroyInvalid
};
template <class TString>
inline TString CreateStatusToString(CreateStatus status) {
    switch (status) {
    case CreateSuccess: return TString("CreateSuccess");
    case CreateFailed: return TString("CreateFailed");
    case CreateBusy: return TString("CreateBusy");
    case CreateInterrupted: return TString("CreateInterrupted");
    case CreateInvalid: return TString("CreateInvalid");

    case DestroyFailed: return TString("DestroyFailed");
    case DestroyBusy: return TString("DestroyBusy");
    case DestroyInterrupted: return TString("DestroyInterrupted");
    case DestroyInvalid: return TString("DestroyInvalid");
    }
    return TString("Unknown");
}

///////////////////////////////////////////////////////////////////////
///  Class: CreateExceptionT
///////////////////////////////////////////////////////////////////////
template 
<typename TStatus = CreateStatus, 
 class TString = String, 
 class TImplements = Exception::Implements, 
 class TExtends = ExceptionT<TStatus, TString, TImplements> >

class _EXPORTED_ CreateExceptionT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef CreateExceptionT Derives;

    typedef TStatus status_t;
    typedef TString string_t;
    
    CreateExceptionT(const status_t& status): Extends(status) {
    }
    CreateExceptionT(const Derives &copy): Extends(copy.Status()) {
    }
    CreateExceptionT(): Extends(CreateFailed) {
    }
    virtual ~CreateExceptionT() {
    }

    virtual string_t StatusToString() const {
        return CreateStatusToString<string_t>(this->Status());
    }
};
typedef CreateExceptionT<> CreateException;

///////////////////////////////////////////////////////////////////////
///  Class: CreatorT
///////////////////////////////////////////////////////////////////////
template <class TImplements = ImplementBase>
class _EXPORTED_ CreatorT: virtual public TImplements {
public:
    typedef TImplements Implements;

    virtual bool Create() { 
        return false; 
    }
    virtual bool Destroy() { 
        return false; 
    }
    virtual bool Created() {
        if (!(IsCreated())) {
            return Create();
        }
        return true; 
    }
    virtual bool Destroyed() {
        if ((IsCreated())) {
            return Destroy();
        }
        return true; 
    }

    virtual bool SetIsCreated(bool to = true) { 
        return false; 
    }
    virtual bool IsCreated() const { 
        return false; 
    }
    virtual bool IsDestroyed() const { 
        return !IsCreated(); 
    }
};
typedef CreatorT<> Creator;

///////////////////////////////////////////////////////////////////////
///  Class: CreatedT
///////////////////////////////////////////////////////////////////////
template 
<typename TAttached = pointer_t,
 typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = Creator, class TExtend = Base,
 class TImplements = AttacherT<TAttached, TUnattached, VUnattached, TImplement>,
 class TExtends = AttachedT<TAttached, TUnattached, VUnattached, TImplements, TExtend> >

class _EXPORTED_ CreatedT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef TAttached Attached;
    static const TUnattached Unattached = VUnattached;

    CreatedT(Attached detached, bool isCreated)
    : Extends(detached), _isCreated(isCreated) {
    }
    CreatedT(Attached detached)
    : Extends(detached), _isCreated(false) {
    }
    CreatedT(const CreatedT &copy)
    : Extends(copy), _isCreated(false) {
    }
    CreatedT(): _isCreated(false) {
    }
    virtual ~CreatedT() {
        LOG_DEBUG("this->Destroyed()...");
        if (!(this->Destroyed())) {
            const CreateException e(DestroyFailed);
            LOG_ERROR("...failed on this->Destroyed() throw (const CreateException e(DestroyFailed))...");
            throw (e);
        }
    }

    virtual bool Create() {
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = CreateAttached())) {
            this->SetIsCreated();
            return true;
        }
        return false;
    }
    virtual bool Destroy() {
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->Detach())) {
            if ((DestroyDetached(detached))) {
                return true;
            }
        }
        return false;
    }

    virtual Attached CreateAttached() {
        Attached detached = ((Attached)Unattached);
        if ((detached = CreateDetached())) {
            this->Attach(detached);
        }
        return detached;
    }
    virtual Attached CreateDetached() const {
        Attached detached = ((Attached)Unattached);
        return detached;
    }
    virtual bool DestroyDetached(Attached detached) const {
        if (((Attached)Unattached) != (detached)) {
            bool success = false;
            return success;
        }
        return false;
    }

    virtual Attached AttachCreated(Attached detached, bool isCreated = true) {
        detached = Extends::Attach(detached);
        this->SetIsCreated(isCreated);
        return detached;
    }
    virtual Attached DetachCreated(bool& isCreated) {
        isCreated = this->IsCreated();
        this->SetIsCreated(false);
        return Extends::Detach();
    }
    virtual Attached Attach(Attached detached) {
        detached = Extends::Attach(detached);
        this->SetIsCreated(false);
        return detached;
    }
    virtual Attached Detach() {
        this->SetIsCreated(false);
        return Extends::Detach();
    }

    virtual bool SetIsCreated(bool isCreated = true) {
        _isCreated = isCreated;
        return _isCreated;
    }
    virtual bool IsCreated() const {
        return _isCreated;
    }

protected:
    bool _isCreated;
};
typedef CreatedT<> Created;

} /// namespace xos

#endif /// _XOS_BASE_CREATED_HXX_
