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
///   File: Waited.hxx
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_BASE_WAITED_HXX_
#define _XOS_BASE_WAITED_HXX_

#include "xos/base/Exception.hxx"

namespace xos {

///////////////////////////////////////////////////////////////////////
/// Enum: WaitStatus
///////////////////////////////////////////////////////////////////////
typedef int WaitStatus;
enum {
    ContinueSuccess,
    WaitSuccess = ContinueSuccess,
    WaitFailed,
    ContinueFailed,

    WaitBusy,
    WaitInterrupted,
    WaitInvalid,

    ContinueBusy,
    ContinueInterrupted,
    ContinueInvalid
};
template <class TString>
inline TString WaitStatusToString(WaitStatus status) {
    switch (status) {
    case WaitSuccess: return TString("WaitSuccess");
    case WaitFailed: return TString("WaitFailed");
    case ContinueFailed: return TString("ContinueFailed");

    case WaitBusy: return TString("WaitBusy");
    case WaitInterrupted: return TString("WaitInterrupted");
    case WaitInvalid: return TString("WaitInvalid");

    case ContinueBusy: return TString("ContinueBusy");
    case ContinueInterrupted: return TString("ContinueInterrupted");
    case ContinueInvalid: return TString("ContinueInvalid");
    }
    return TString("Unknown");
}

///////////////////////////////////////////////////////////////////////
///  Class: WaitExceptionT
///////////////////////////////////////////////////////////////////////
template 
<typename TStatus = WaitStatus, 
 class TString = String, 
 class TImplements = Exception::Implements, 
 class TExtends = ExceptionT<TStatus, TString, TImplements> >

class _EXPORTED_ WaitExceptionT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef WaitExceptionT Derives;

    typedef TStatus status_t;
    typedef TString string_t;
    
    WaitExceptionT(const status_t& status): Extends(status) {
    }
    WaitExceptionT(const Derives &copy): Extends(copy.Status()) {
    }
    WaitExceptionT(): Extends(WaitFailed) {
    }
    virtual ~WaitExceptionT() {
    }

    virtual string_t StatusToString() const {
        return WaitStatusToString<string_t>(this->Status());
    }
};
typedef WaitExceptionT<> WaitException;

///////////////////////////////////////////////////////////////////////
///  Class: WaitedT
///////////////////////////////////////////////////////////////////////
template <class TImplements = ImplementBase>
class _EXPORTED_ WaitedT: virtual public TImplements {
public:
    typedef TImplements Implements;

    virtual bool Wait() { 
        if (WaitSuccess == (UntimedWait())) {
            return true;
        }
        return false; 
    }
    virtual WaitStatus TryWait() { 
        return WaitFailed; 
    }
    virtual WaitStatus TimedWait(mseconds_t milliseconds) { 
        return WaitFailed; 
    }
    virtual WaitStatus UntimedWait() { 
        return WaitFailed; 
    }
    
    virtual bool Continue() { 
        if (ContinueSuccess == (UntimedContinue())) {
            return true;
        }
        return false; 
    }
    virtual WaitStatus TryContinue() { 
        return ContinueFailed; 
    }
    virtual WaitStatus TimedContinue(mseconds_t milliseconds) { 
        return ContinueFailed; 
    }
    virtual WaitStatus UntimedContinue() { 
        return ContinueFailed; 
    }
};
typedef WaitedT<> Waited;

///////////////////////////////////////////////////////////////////////
///  Class: WaitT
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = Waited, class TExtends = Base>
class _EXPORTED_ WaitT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    WaitT(Waited& waited, mseconds_t milliseconds): _waited(waited) {
        WaitStatus status = WaitFailed;
        if (WaitSuccess != (status = _waited.TimedWait(milliseconds))) {
            const WaitException e(status);
            throw (e); 
        }
    }
    WaitT(Waited& waited): _waited(waited) {
        if (!(_waited.Wait())) {
            const WaitException e(WaitFailed);
            throw (e);
        }
    }
    virtual ~WaitT() {
    }
private:
    WaitT(const WaitT &copy): _waited(*this) {
    }
    WaitT(): _waited(*this) {
    }

protected:
    Waited& _waited;
};
typedef WaitT<> Wait;

///////////////////////////////////////////////////////////////////////
///  Class: TryWaitT
///////////////////////////////////////////////////////////////////////
template
<class TImplements = Waited, class TExtends = Base>
class _EXPORTED_ TryWaitT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    TryWaitT(Waited& waited): _waited(waited) {
        WaitStatus status = WaitFailed;
        if (WaitSuccess != (status = _waited.TryWait())) {
            const WaitException e(status);
            throw (e);
        }
    }
    virtual ~TryWaitT() {
    }
private:
    TryWaitT(const TryWaitT &copy): _waited(*this) {
    }
    TryWaitT(): _waited(*this) {
    }

protected:
    Waited& _waited;
};
typedef TryWaitT<> TryWait;

} /// namespace xos

#endif /// _XOS_BASE_WAITED_HXX_
