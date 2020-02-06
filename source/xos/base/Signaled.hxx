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
///   File: Signaled.hxx
///
/// Author: $author$
///   Date: 6/10/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_SIGNALED_HXX_
#define _XOS_SIGNALED_HXX_

#include "xos/base/Waited.hxx"

namespace xos {

///////////////////////////////////////////////////////////////////////
/// Enum: SignalStatus
///////////////////////////////////////////////////////////////////////
typedef WaitStatus SignalStatus;
enum {
    SignalSuccess = WaitSuccess,
    SignalFailed = ContinueFailed,
    SignalBusy = ContinueBusy,
    SignalInterrupted = ContinueInterrupted,
    SignalInvalid = ContinueInvalid
};
template <class TString>
inline TString SignalStatusToString(SignalStatus status) {
    switch (status) {
    case SignalSuccess: return TString("SignalSuccess");
    case SignalFailed: return TString("SignalFailed");
    case SignalBusy: return TString("SignalBusy");
    case SignalInterrupted: return TString("SignalInterrupted");
    case SignalInvalid: return TString("SignalInvalid");
    }
    return TString(WaitStatusToString<TString>(status));
}

///////////////////////////////////////////////////////////////////////
///  Class: SignalExceptionT
///////////////////////////////////////////////////////////////////////
template 
<typename TStatus = SignalStatus, 
 class TString = String, 
 class TImplements = Exception::Implements, 
 class TExtends = ExceptionT<TStatus, TString, TImplements> >

class _EXPORTED_ SignalExceptionT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef SignalExceptionT Derives;

    typedef TStatus status_t;
    typedef TString string_t;
    
    SignalExceptionT(const status_t& status): Extends(status) {
    }
    SignalExceptionT(const Derives &copy): Extends(copy.Status()) {
    }
    SignalExceptionT(): Extends(SignalFailed) {
    }
    virtual ~SignalExceptionT() {
    }

    virtual string_t StatusToString() const {
        return ExceptionStatusToString<string_t>(this->Status());
    }
};
typedef SignalExceptionT<> SignalException;

///////////////////////////////////////////////////////////////////////
///  Class: SignaledT
///////////////////////////////////////////////////////////////////////
template <class TImplements = Waited>
class _EXPORTED_ SignaledT: virtual public TImplements {
public:
    typedef TImplements Implements;

    virtual bool Signal() { 
        return false; 
    }
    virtual SignalStatus TrySignal() { 
        return SignalFailed; 
    }
    virtual SignalStatus TimedSignal(mseconds_t milliseconds) { 
        return SignalFailed; 
    }
    virtual SignalStatus UntimedSignal() { 
        return SignalFailed; 
    }
}; /// class _EXPORTED_ SignaledT
typedef SignaledT<> Signaled;

///////////////////////////////////////////////////////////////////////
///  Class: SignalT
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = Signaled, class TExtends = Base>
class _EXPORTED_ SignalT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    SignalT(Signaled& signaled, mseconds_t milliseconds): _signaled(signaled) {
        SignalStatus status = SignalFailed;
        if (SignalSuccess != (status = _signaled.TimedSignal(milliseconds))) {
            const SignalException e(status);
            throw (e); 
        }
    }
    SignalT(Signaled& signaled): _signaled(signaled) {
        if (!(_signaled.Signal())) {
            const SignalException e(SignalFailed);
            throw (e);
        }
    }
    virtual ~SignalT() {
    }
private:
    SignalT(const SignalT &copy): _signaled(*this) {
    }
    SignalT(): _signaled(*this) {
    }

protected:
    Signaled& _signaled;
};
typedef SignalT<> Signal;

} /// namespace xos

#endif /// _XOS_SIGNALED_HXX_
