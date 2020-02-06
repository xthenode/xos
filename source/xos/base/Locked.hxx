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
///   File: Locked.hxx
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_BASE_LOCKED_HXX_
#define _XOS_BASE_LOCKED_HXX_

#include "xos/base/Exception.hxx"

namespace xos {

typedef int LockStatus;
enum {
    UnlockSuccess,
    LockSuccess = UnlockSuccess,
    LockFailed,
    UnlockFailed,

    LockBusy,
    LockInterrupted,
    LockInvalid,

    UnlockBusy,
    UnlockInterrupted,
    UnlockInvalid
};
template <class TString>
inline TString LockStatusToString(LockStatus status) {
    switch (status) {
    case LockSuccess: return TString("LockSuccess");
    case LockFailed: return TString("LockFailed");
    case UnlockFailed: return TString("UnlockFailed");
    case LockBusy: return TString("LockBusy");
    case LockInterrupted: return TString("LockInterrupted");
    case LockInvalid: return TString("LockInvalid");
    case UnlockBusy: return TString("UnlockBusy");
    case UnlockInterrupted: return TString("UnlockInterrupted");
    case UnlockInvalid: return TString("UnlockInvalid");
    default: break;
    }
    return TString("Unknown");
}
///////////////////////////////////////////////////////////////////////
///  Class: LockExceptionT
///////////////////////////////////////////////////////////////////////
template 
<typename TStatus = LockStatus, 
 class TString = String, 
 class TImplements = Exception::Implements, 
 class TExtends = ExceptionT<TStatus, TString, TImplements> >

class _EXPORTED_ LockExceptionT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef LockExceptionT Derives;

    typedef TStatus status_t;
    typedef TString string_t;
    
    LockExceptionT(const status_t& status): Extends(status) {
    }
    LockExceptionT(const Derives &copy): Extends(copy.Status()) {
    }
    LockExceptionT(): Extends(LockFailed) {
    }
    virtual ~LockExceptionT() {
    }

    virtual string_t StatusToString() const {
        return LockStatusToString<string_t>(this->Status());
    }
};
typedef LockExceptionT<> LockException;

typedef ImplementBase LockedTImplements;
///////////////////////////////////////////////////////////////////////
///  Class: LockedT
///////////////////////////////////////////////////////////////////////
template <class TImplements = LockedTImplements>
class _EXPORTED_ LockedT: virtual public TImplements {
public:
    typedef TImplements Implements;

    virtual bool Lock() { 
        return false; 
    }
    virtual LockStatus TryLock() { 
        return LockFailed; 
    }
    virtual LockStatus TimedLock(mseconds_t milliseconds) { 
        return LockFailed; 
    }
    virtual LockStatus UntimedLock() { 
        return LockFailed; 
    }
    virtual bool Unlock() { 
        return false; 
    }
};
typedef LockedT<> Locked;

typedef Locked UnlockedTImplements;
///////////////////////////////////////////////////////////////////////
///  Class: UnlockedT
///////////////////////////////////////////////////////////////////////
template <class TImplements = UnlockedTImplements>
class _EXPORTED_ UnlockedT: virtual public TImplements {
public:
    typedef TImplements Implements;

    virtual bool Lock() { 
        return true; 
    }
    virtual LockStatus TryLock() { 
        return LockSuccess; 
    }
    virtual LockStatus TimedLock(mseconds_t milliseconds) { 
        return LockSuccess; 
    }
    virtual LockStatus UntimedLock() { 
        return LockSuccess; 
    }
    virtual bool Unlock() { 
        return true; 
    }
};
typedef UnlockedT<> Unlocked;

typedef Unlocked LockTImplements;
typedef Base LockTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: LockT
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = LockTImplements, class TExtends = LockTExtends>
class _EXPORTED_ LockT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    LockT(Locked& locked, mseconds_t milliseconds): _locked(locked), _unlocked(false) {
        LockStatus status = LockFailed;
        if (LockSuccess != (status = _locked.TimedLock(milliseconds))) {
            const LockException e(status);
            throw (e); 
        }
    }
    LockT(Locked& locked): _locked(locked), _unlocked(false) {
        if (!(_locked.Lock())) {
            const LockException e(LockFailed);
            throw (e);
        }
    }
    virtual ~LockT() {
        if (!(Unlocked())) {
            SetUnlocked();
            if (!(_locked.Unlock())) {
                const LockException e(UnlockFailed);
                throw (e);
            }
        }
    }
private:
    LockT(const LockT &copy): _locked(*this), _unlocked(false) {
    }
    LockT(): _locked(*this), _unlocked(false) {
    }

public:
    virtual bool SetUnlocked(bool to = true) {
        return _unlocked = to;
    }
    virtual bool Unlocked() const {
        return _unlocked;
    }

protected:
    Locked& _locked;
    bool _unlocked;
};
typedef LockT<> Lock;

} /// namespace xos

#endif /// _XOS_BASE_LOCKED_HXX_
