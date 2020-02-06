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
///   File: Opened.hxx
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_BASE_OPENED_HXX_
#define _XOS_BASE_OPENED_HXX_

#include "xos/base/Attached.hxx"

namespace xos {

typedef int OpenStatus;
enum {
    CloseSuccess,
    OpenSuccess = CloseSuccess,
    OpenFailed,
    CloseFailed,

    OpenBusy,
    OpenInterrupted,
    OpenInvalid,

    CloseBusy,
    CloseInterrupted,
    CloseInvalid
};
template <class TString>
inline const char* OpenStatusToString(OpenStatus status) {
    switch (status) {
    case OpenSuccess: return TString("OpenSuccess");
    case OpenFailed: return TString("OpenFailed");
    case CloseFailed: return TString("CloseFailed");
    case OpenBusy: return TString("OpenBusy");
    case OpenInterrupted: return TString("OpenInterrupted");
    case OpenInvalid: return TString("OpenInvalid");
    case CloseBusy: return TString("CloseBusy");
    case CloseInterrupted: return TString("CloseInterrupted");
    case CloseInvalid: return TString("CloseInvalid");
    default: break;
    }
    return TString("Unknown");
}
///////////////////////////////////////////////////////////////////////
///  Class: OpenExceptionT
///////////////////////////////////////////////////////////////////////
template 
<typename TStatus = OpenStatus, 
 class TString = String, 
 class TImplements = Exception::Implements, 
 class TExtends = ExceptionT<TStatus, TString, TImplements> >

class _EXPORTED_ OpenExceptionT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef OpenExceptionT Derives;

    typedef TStatus status_t;
    typedef TString string_t;
    
    OpenExceptionT(const status_t& status): Extends(status) {
    }
    OpenExceptionT(const Derives &copy): Extends(copy.Status()) {
    }
    OpenExceptionT(): Extends(OpenFailed) {
    }
    virtual ~OpenExceptionT() {
    }

    virtual string_t StatusToString() const {
        return ExceptionStatusToString<string_t>(this->Status());
    }
};
typedef OpenExceptionT<> OpenException;

typedef ImplementBase OpenerTImplements;
///////////////////////////////////////////////////////////////////////
///  Class: OpenerT
///////////////////////////////////////////////////////////////////////
template <class TImplements = OpenerTImplements>
class _EXPORTED_ OpenerT: virtual public TImplements {
public:
    typedef TImplements Implements;

    virtual bool Open() {
        return false;
    }
    virtual bool Close() {
        return false;
    }
    virtual bool Opened() {
        if (!(this->IsOpen())) {
            return this->Open();
        }
        return true;
    }
    virtual bool Closed() {
        if ((this->IsOpen())) {
            return this->Close();
        }
        return true;
    }

    virtual bool SetIsOpen(bool to = true) {
        return this->IsOpen();
    }
    virtual bool IsOpen() const {
        return false;
    }
    virtual bool IsClosed() const {
        return !this->IsOpen();
    }
};
typedef OpenerT<> Opener;

typedef Opener OpenedTImplements;
typedef Base OpenedTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: OpenedT
///////////////////////////////////////////////////////////////////////
template 
<typename TAttached = pointer_t,
 typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = OpenedTImplements, class TExtend = OpenedTExtends,
 class TImplements = AttacherT<TAttached, TUnattached, VUnattached, TImplement>,
 class TExtends = AttachedT<TAttached, TUnattached, VUnattached, TImplements, TExtend> >

class _EXPORTED_ OpenedT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef TAttached Attached;
    static const TUnattached Unattached = VUnattached;

    OpenedT(const OpenedT &copy): _isOpen(copy._isOpen) {
    }
    OpenedT(): _isOpen(false) {
    }
    virtual ~OpenedT() {
        LOG_DEBUG("this->Closed()...");
        if (!(this->Closed())) {
            const OpenException e(CloseFailed);
            LOG_ERROR("...failed this->Closed() on throw (const OpenException e(CloseFailed))..");
            throw (e);
        }
    }

    virtual bool Open() {
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = OpenAttached())) {
            this->SetIsOpen();
            return true;
        }
        return false;
    }
    virtual bool Close() {
        Attached detached = ((Attached)Unattached);
        if (((Attached)Unattached) != (detached = this->Detach())) {
            if ((CloseDetached(detached))) {
                return true;
            }
        }
        return false;
    }

    virtual Attached OpenAttached() {
        Attached detached = ((Attached)Unattached);
        if ((detached = OpenDetached())) {
            this->Attach(detached);
        }
        return detached;
    }
    virtual Attached OpenDetached() const {
        Attached detached = ((Attached)Unattached);
        return detached;
    }
    virtual bool CloseDetached(Attached detached) const {
        if (((Attached)Unattached) != (detached)) {
            bool success = false;
            return success;
        }
        return false;
    }

    virtual Attached AttachOPened(Attached attachedTo, bool isOpen = true) {
        attachedTo = Extends::Attach(attachedTo);
        this->SetIsOpen(isOpen);
        return attachedTo;
    }
    virtual Attached DetachOpened(bool& isOpen) {
        isOpen = this->IsOpen();
        this->SetIsOpen(false);
        return Extends::Detach();
    }
    virtual Attached Attach(Attached attachedTo) {
        attachedTo = Extends::Attach(attachedTo);
        this->SetIsOpen(false);
        return attachedTo;
    }
    virtual Attached Detach() {
        this->SetIsOpen(false);
        return Extends::Detach();
    }

    virtual bool SetIsOpen(bool to = true) {
        _isOpen = to;
        return _isOpen;
    }
    virtual bool IsOpen() const {
        return _isOpen;
    }

protected:
    bool _isOpen;
};
typedef OpenedT<> Opened;

} /// namespace xos

#endif /// _XOS_BASE_OPENED_HXX_
