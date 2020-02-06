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
///   File: Attached.hxx
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_BASE_ATTACHED_HXX_
#define _XOS_BASE_ATTACHED_HXX_

#include "xos/base/Exception.hxx"
#include "xos/logger/Interface.hxx"

namespace xos {

typedef int AttachStatus;
enum {
    DetachSuccess,
    AttachSuccess = DetachSuccess,
    AttachFailed,
    DetachFailed,
    
    AttachBusy,
    AttachInterrupted,
    AttachInvalid,

    DetachBusy,
    DetachInterrupted,
    DetachInvalid
};

template <class TString>
inline TString AttachStatusToString(AttachStatus status) {
    switch (status) {
    case AttachSuccess: return TString("AttachSuccess");
    case AttachFailed: return TString("AttachFailed");
    case AttachBusy: return TString("AttachBusy");
    case AttachInterrupted: return TString("AttachInterrupted");
    case AttachInvalid: return TString("AttachInvalid");

    case DetachFailed: return TString("DetachFailed");
    case DetachBusy: return TString("DetachBusy");
    case DetachInterrupted: return TString("DetachInterrupted");
    case DetachInvalid: return TString("DetachInvalid");
    }
    return TString("Unknown");
}
///////////////////////////////////////////////////////////////////////
///  Class: AttachExceptionT
///////////////////////////////////////////////////////////////////////
template 
<typename TStatus = AttachStatus, 
 class TString = String, 
 class TImplements = Exception::Implements, 
 class TExtends = ExceptionT<TStatus, TString, TImplements> >

class _EXPORTED_ AttachExceptionT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef AttachExceptionT Derives;

    typedef TStatus status_t;
    typedef TString string_t;
    
    AttachExceptionT(const status_t& status): Extends(status) {
    }
    AttachExceptionT(const Derives &copy): Extends(copy.Status()) {
    }
    AttachExceptionT(): Extends(AttachFailed) {
    }
    virtual ~AttachExceptionT() {
    }

    virtual string_t StatusToString() const {
        return ExceptionStatusToString<string_t>(this->Status());
    }
};
typedef AttachExceptionT<> AttachException;

///////////////////////////////////////////////////////////////////////
///  Class: AttacherT
///////////////////////////////////////////////////////////////////////
template 
<typename TAttached = pointer_t,
 typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplements = ImplementBase>

class _EXPORTED_ AttacherT: virtual public TImplements {
public:
    typedef TImplements Implements;

    typedef TAttached Attached;
    static const TUnattached Unattached = VUnattached;

    virtual Attached Attach(Attached attachedTo) {
        return ((Attached)Unattached);
    }
    virtual Attached Detach() {
        return ((Attached)Unattached);
    }
    virtual bool Detached() {
        if (((Attached)Unattached) != (this->AttachedTo())) {
            if (((Attached)Unattached) == (this->Detach())) {
                return false;
            }
        }
        return true;
    }
    virtual Attached AttachedTo() const {
        return ((Attached)Unattached);
    }
    virtual operator Attached() const {
        return AttachedTo();
    }
};
typedef AttacherT<> Attacher;

///////////////////////////////////////////////////////////////////////
///  Class: AttachedT
///////////////////////////////////////////////////////////////////////
template 
<typename TAttached = pointer_t,
 typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = ImplementBase, class TExtends = Base,
 class TImplements = AttacherT<TAttached, TUnattached, VUnattached, TImplement> >

class _EXPORTED_ AttachedT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef TAttached Attached;
    static const TUnattached Unattached = VUnattached;

    AttachedT(Attached detached): _attachedTo(detached) {
    }
    AttachedT(const AttachedT &copy): _attachedTo(copy.AttachedTo()) {
    }
    AttachedT(): _attachedTo(((Attached)Unattached)) {
    }
    virtual ~AttachedT() {
        LOG_DEBUG("this->Detached()...");
        if (!(this->Detached())) {
            const AttachException e(DetachFailed);
            LOG_ERROR("...failed on this->Detached() throw (const AttachException e(DetachFailed))...");
            throw (e);
        }
    }

    virtual Attached Attach(Attached attachedTo) {
        return _attachedTo = attachedTo;
    }
    virtual Attached Detach() {
        Attached detached = _attachedTo;
        _attachedTo = ((Attached)Unattached);
        return detached;
    }
    virtual Attached AttachedTo() const {
        return _attachedTo;
    }

protected:
    Attached _attachedTo;
};
typedef AttachedT<> Attached;

} /// namespace xos

#endif /// _XOS_BASE_ATTACHED_HXX_
