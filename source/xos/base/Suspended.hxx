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
///   File: Suspended.hxx
///
/// Author: $author$
///   Date: 5/31/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_BASE_SUSPENDED_HXX_
#define _XOS_BASE_SUSPENDED_HXX_

#include "xos/base/Exception.hxx"

namespace xos {

typedef int SuspendStatus;
enum {
    ResumeSuccess,
    SuspendSuccess = ResumeSuccess,

    SuspendFailed,
    SuspendBusy,
    SuspendInterrupted,
    SuspendInvalid,

    ResumeFailed,
    ResumeBusy,
    ResumeInterrupted,
    ResumeInvalid
};

template <class TString>
inline TString SuspendStatusToString(SuspendStatus status) {
    switch (status) {
    case SuspendSuccess: return TString("SuspendSuccess");
    case SuspendFailed: return TString("SuspendFailed");
    case SuspendBusy: return TString("SuspendBusy");
    case SuspendInterrupted: return TString("SuspendInterrupted");
    case SuspendInvalid: return TString("SuspendInvalid");

    case ResumeFailed: return TString("ResumeFailed");
    case ResumeBusy: return TString("ResumeBusy");
    case ResumeInterrupted: return TString("ResumeInterrupted");
    case ResumeInvalid: return TString("ResumeInvalid");
    }
    return TString("Unknown");
}

///////////////////////////////////////////////////////////////////////
///  Class: SuspendExceptionT
///////////////////////////////////////////////////////////////////////
template 
<typename TStatus = SuspendStatus, class TString = String, 
 class TImplements = Exception::Implements, class TExtends = ExceptionT<TStatus, TString, TImplements> >

class _EXPORTED_ SuspendExceptionT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef SuspendExceptionT Derives;

    typedef TStatus status_t;
    typedef TString string_t;

    SuspendExceptionT(const status_t& status): Extends(status) {
    }
    SuspendExceptionT(const SuspendExceptionT& copy): Extends(copy.Status()) {
    }
    SuspendExceptionT(): Extends(SuspendFailed) {
    }
    virtual ~SuspendExceptionT() {
    }

    virtual string_t StatusToString() const {
        return SuspendStatusToString<string_t>(this->Status());
    }
};
typedef SuspendExceptionT<> SuspendException;

///////////////////////////////////////////////////////////////////////
///  Class: SuspendedT
///////////////////////////////////////////////////////////////////////
template 
<class TSuspendException = SuspendException, class TImplements = ImplementBase>
class _EXPORTED_ SuspendedT: virtual public TImplements {
public:
    typedef TImplements Implements;
    typedef TSuspendException SuspendException;

    virtual bool Suspended() {
        if ((this->IsResumed())) {
            return this->Suspend();
        }
        return true;
    }
    virtual bool Suspend() { 
        return false; 
    }
    virtual SuspendStatus TrySuspend() { 
        return SuspendFailed; 
    }
    virtual SuspendStatus TimedSuspend(mseconds_t milliseconds) { 
        return SuspendFailed; 
    }
    virtual SuspendStatus UntimedSuspend() { 
        return SuspendFailed; 
    }

    virtual bool Resumed() {
        if ((this->IsSuspended())) {
            return this->Resume();
        }
        return true;
    }
    virtual bool Resume() { 
        return false; 
    }
    virtual SuspendStatus TryResume() { 
        return ResumeFailed; 
    }
    virtual SuspendStatus TimedResume(mseconds_t milliseconds) { 
        return ResumeFailed; 
    }
    virtual SuspendStatus UntimedResume() { 
        return ResumeFailed; 
    }

    virtual bool SetIsResumed(bool to = true) { 
        return false; 
    }
    virtual bool IsResumed() const { 
        return false; 
    }
    virtual bool IsSuspended() const { 
        return !IsResumed(); 
    }
};
typedef SuspendedT<> Suspended;

///////////////////////////////////////////////////////////////////////
///  Class: SuspendT
///////////////////////////////////////////////////////////////////////
template <class TImplements = Suspended, class TExtends = Base>
class _EXPORTED_ SuspendT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    SuspendT(Suspended& suspended, mseconds_t milliseconds): _suspended(suspended) {
        SuspendStatus status = SuspendFailed;
        if (SuspendSuccess != (status = _suspended.TimedSuspend(milliseconds))) {
            throw SuspendException(status); 
        }
    }
    SuspendT(Suspended& suspended): _suspended(suspended) {
        if (!(_suspended.Suspend())) {
            throw SuspendException(SuspendFailed);
        }
    }
    virtual ~SuspendT() {
        if (!(_suspended.Resume())) {
            throw SuspendException(ResumeFailed);
        }
    }
private:
    SuspendT(const SuspendT &copy): _suspended(*this) {
    }
    SuspendT(): _suspended(*this) {
    }

protected:
    Suspended& _suspended;
};
typedef SuspendT<> Suspend;

} /// namespace xos

#endif /// _XOS_BASE_SUSPENDED_HXX_
