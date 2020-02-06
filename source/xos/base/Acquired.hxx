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
///   File: Acquired.hxx
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_BASE_ACQUIRED_HXX_
#define _XOS_BASE_ACQUIRED_HXX_

#include "xos/base/Exception.hxx"

namespace xos {

///////////////////////////////////////////////////////////////////////
/// Enum: AcquireStatus
///////////////////////////////////////////////////////////////////////
typedef int AcquireStatus;
enum {
    ReleaseSuccess,
    AcquireSuccess = ReleaseSuccess,
    AcquireFailed,
    ReleaseFailed,

    AcquireBusy,
    AcquireInterrupted,
    AcquireInvalid,

    ReleaseBusy,
    ReleaseInterrupted,
    ReleaseInvalid
};
template <class TString>
inline TString AcquireStatusToString(AcquireStatus status) {
    switch (status) {
    case AcquireSuccess: return TString("AcquireSuccess");
    case AcquireFailed: return TString("AcquireFailed");
    case ReleaseFailed: return TString("ReleaseFailed");

    case AcquireBusy: return TString("AcquireBusy");
    case AcquireInterrupted: return TString("AcquireInterrupted");
    case AcquireInvalid: return TString("AcquireInvalid");

    case ReleaseBusy: return TString("ReleaseBusy");
    case ReleaseInterrupted: return TString("ReleaseInterrupted");
    case ReleaseInvalid: return TString("ReleaseInvalid");
    }
    return TString("Unknown");
}

///////////////////////////////////////////////////////////////////////
///  Class: AcquireExceptionT
///////////////////////////////////////////////////////////////////////
template 
<typename TStatus = AcquireStatus, 
 class TString = String, 
 class TImplements = Exception::Implements, 
 class TExtends = ExceptionT<TStatus, TString, TImplements> >

class _EXPORTED_ AcquireExceptionT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef AcquireExceptionT Derives;

    typedef TStatus status_t;
    typedef TString string_t;
    
    AcquireExceptionT(const status_t& status): Extends(status) {
    }
    AcquireExceptionT(const Derives &copy): Extends(copy.Status()) {
    }
    AcquireExceptionT(): Extends(AcquireFailed) {
    }
    virtual ~AcquireExceptionT() {
    }

    virtual string_t StatusToString() const {
        return AcquireStatusToString<string_t>(this->Status());
    }
};
typedef AcquireExceptionT<> AcquireException;

///////////////////////////////////////////////////////////////////////
///  Class: AcquiredT
///////////////////////////////////////////////////////////////////////
template <class TImplements = ImplementBase>
class _EXPORTED_ AcquiredT: virtual public TImplements {
public:
    typedef TImplements Implements;

    virtual bool Acquire() { 
        return false; 
    }
    virtual AcquireStatus TryAcquire() { 
        return AcquireFailed; 
    }
    virtual AcquireStatus TimedAcquire(mseconds_t milliseconds) { 
        return AcquireFailed; 
    }
    virtual AcquireStatus UntimedAcquire() { 
        return AcquireFailed; 
    }
    virtual bool Release() { 
        return false; 
    }
    virtual AcquireStatus TryRelease() { 
        return ReleaseFailed; 
    }
    virtual AcquireStatus TimedRelease(mseconds_t milliseconds) { 
        return ReleaseFailed; 
    }
    virtual AcquireStatus UntimedRelease() { 
        return ReleaseFailed; 
    }
};
typedef AcquiredT<> Acquired;

///////////////////////////////////////////////////////////////////////
///  Class: ReleasedT
///////////////////////////////////////////////////////////////////////
template <class TImplements = Acquired>
class _EXPORTED_ ReleasedT: virtual public TImplements {
public:
    typedef TImplements Implements;

    virtual bool Acquire() { 
        return true; 
    }
    virtual AcquireStatus TryAcquire() { 
        return AcquireSuccess; 
    }
    virtual AcquireStatus TimedAcquire(mseconds_t milliseconds) { 
        return AcquireSuccess; 
    }
    virtual AcquireStatus UntimedAcquire() { 
        return AcquireSuccess; 
    }
    virtual bool Release() { 
        return true; 
    }
    virtual AcquireStatus TryRelease() { 
        return ReleaseSuccess; 
    }
    virtual AcquireStatus TimedRelease(mseconds_t milliseconds) { 
        return ReleaseSuccess; 
    }
    virtual AcquireStatus UntimedRelease() { 
        return ReleaseSuccess; 
    }
};
typedef ReleasedT<> Released;

///////////////////////////////////////////////////////////////////////
///  Class: AcquireT
///////////////////////////////////////////////////////////////////////
template
<class TImplements = Released, class TExtends = Base>
class _EXPORTED_ AcquireT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    AcquireT(Acquired& acquired, mseconds_t milliseconds): _acquired(acquired) {
        AcquireStatus status = AcquireSuccess;
        if (AcquireSuccess != (status = _acquired.TimedAcquire(milliseconds))) {
            AcquireException e(status);
            throw (e);
        }
    }
    AcquireT(Acquired& acquired): _acquired(acquired) {
        if (!(_acquired.Acquire())) {
            AcquireException e(AcquireFailed);
            throw (e);
        }
    }
    virtual ~AcquireT() {
    }
private:
    AcquireT(const AcquireT& copy): _acquired(*this) {
    }
    AcquireT(): _acquired(*this) {
    }

protected:
    Acquired& _acquired;
};
typedef AcquireT<> Acquire;

///////////////////////////////////////////////////////////////////////
///  Class: TryAcquireT
///////////////////////////////////////////////////////////////////////
template
<class TImplements = Released, class TExtends = Base>
class _EXPORTED_ TryAcquireT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    TryAcquireT(Acquired& acquired): _acquired(acquired) {
        AcquireStatus status = AcquireSuccess;
        if (AcquireSuccess != (status = _acquired.TryAcquire())) {
            AcquireException e(status);
            throw (e);
        }
    }
    virtual ~TryAcquireT() {
    }
private:
    TryAcquireT(const TryAcquireT& copy): _acquired(*this) {
    }
    TryAcquireT(): _acquired(*this) {
    }

protected:
    Acquired& _acquired;
};
typedef TryAcquireT<> TryAcquire;

///////////////////////////////////////////////////////////////////////
///  Class: ReleaseT
///////////////////////////////////////////////////////////////////////
template <class TImplements = Released, class TExtends = Base>
class _EXPORTED_ ReleaseT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    ReleaseT(Acquired& acquired, mseconds_t milliseconds): _acquired(acquired) {
        AcquireStatus status = ReleaseSuccess;
        if (ReleaseSuccess != (status = _acquired.TimedRelease(milliseconds))) {
            AcquireException e(ReleaseFailed);
            throw (e);
        }
    }
    ReleaseT(Acquired& acquired): _acquired(acquired) {
        if (!(_acquired.Release())) {
            AcquireException e(ReleaseFailed);
            throw (e);
        }
    }
    virtual ~ReleaseT() {
    }
private:
    ReleaseT(const ReleaseT& copy): _acquired(*this) {
    }
    ReleaseT(): _acquired(*this) {
    }

protected:
    Acquired& _acquired;
};
typedef ReleaseT<> Release;

} /// namespace xos

#endif /// _XOS_BASE_ACQUIRED_HXX_
