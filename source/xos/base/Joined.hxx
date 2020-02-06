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
///   File: Joined.hxx
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_BASE_JOINED_HXX_
#define _XOS_BASE_JOINED_HXX_

#include "xos/base/Exception.hxx"

namespace xos {

typedef int JoinStatus;
enum {
    ForkSuccess,
    JoinSuccess = ForkSuccess,

    JoinFailed,
    JoinBusy,
    JoinInterrupted,
    JoinInvalid,

    ForkFailed,
    ForkBusy,
    ForkInterrupted,
    ForkInvalid
};

template <class TString>
inline TString JoinStatusToString(JoinStatus status) {
    switch (status) {
    case JoinSuccess: return TString("JoinSuccess");
    case JoinFailed: return TString("JoinFailed");
    case JoinBusy: return TString("JoinBusy");
    case JoinInterrupted: return TString("JoinInterrupted");
    case JoinInvalid: return TString("JoinInvalid");

    case ForkFailed: return TString("ForkFailed");
    case ForkBusy: return TString("ForkBusy");
    case ForkInterrupted: return TString("ForkInterrupted");
    case ForkInvalid: return TString("ForkInvalid");
    }
    return TString("Unknown");
}

///////////////////////////////////////////////////////////////////////
///  Class: JoinExceptionT
///////////////////////////////////////////////////////////////////////
template 
<typename TStatus = JoinStatus, 
 class TString = String, 
 class TImplements = Exception::Implements, 
 class TExtends = ExceptionT<TStatus, TString, TImplements> >

class _EXPORTED_ JoinExceptionT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef JoinExceptionT Derives;

    typedef TStatus status_t;
    typedef TString string_t;

    JoinExceptionT(const status_t& status): Extends(status) {
    }
    JoinExceptionT(const JoinExceptionT& copy): Extends(copy.Status()) {
    }
    JoinExceptionT(): Extends(JoinFailed) {
    }
    virtual ~JoinExceptionT() {
    }

    virtual string_t StatusToString() const {
        return JoinStatusToString<string_t>(this->Status());
    }
};
typedef JoinExceptionT<> JoinException;

///////////////////////////////////////////////////////////////////////
///  Class: JoinedT
///////////////////////////////////////////////////////////////////////
template
<class TJoinException = JoinException, class TImplements = ImplementBase>
class _EXPORTED_ JoinedT: virtual public TImplements {
public:
    typedef TImplements Implements;
    typedef TJoinException JoinException;

    virtual bool Joined() {
        if ((this->IsForked())) {
            if ((this->Join())) {
                return true;
            }
        }
        return true;
    }
    virtual bool Join() { 
        return false; 
    }
    virtual JoinStatus TryJoin() { 
        return JoinFailed; 
    }
    virtual JoinStatus TimedJoin(mseconds_t milliseconds) { 
        return JoinFailed; 
    }
    virtual JoinStatus UntimedJoin() { 
        return JoinFailed; 
    }

    virtual bool Forked() {
        if ((this->IsJoined())) {
            if ((this->Fork())) {
                return true;
            }
        }
        return true;
    }
    virtual bool Fork() { 
        return false; 
    }
    virtual JoinStatus TryFork() { 
        return ForkFailed; 
    }
    virtual JoinStatus TimedFork(mseconds_t milliseconds) { 
        return ForkFailed; 
    }
    virtual JoinStatus UntimedFork() { 
        return ForkFailed; 
    }

    virtual bool SetIsForked(bool to = true) { 
        return false; 
    }
    virtual bool IsForked() const { 
        return false; 
    }
    virtual bool IsJoined() const { 
        return !IsForked(); 
    }
};
typedef JoinedT<> Joined;

///////////////////////////////////////////////////////////////////////
///  Class: JoinT
///////////////////////////////////////////////////////////////////////
template <class TImplements = Joined, class TExtends = Base>
class _EXPORTED_ JoinT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    JoinT(Joined& joined, mseconds_t milliseconds): _joined(joined) {
        JoinStatus status = JoinFailed;
        if (JoinSuccess != (status = _joined.TimedJoin(milliseconds))) {
            throw JoinException(status); 
        }
    }
    JoinT(Joined& joined): _joined(joined) {
        if (!(_joined.Join())) {
            throw JoinException(JoinFailed);
        }
    }
    virtual ~JoinT() {
    }
private:
    JoinT(const JoinT &copy): _joined(*this) {
    }
    JoinT(): _joined(*this) {
    }

protected:
    Joined& _joined;
};
typedef JoinT<> Join;

///////////////////////////////////////////////////////////////////////
///  Class: TryJoinT
///////////////////////////////////////////////////////////////////////
template <class TImplements = Joined, class TExtends = Base>
class _EXPORTED_ TryJoinT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    TryJoinT(Joined& joined): _joined(joined) {
        JoinStatus status = JoinFailed;
        if (JoinSuccess != (status = _joined.TryJoin())) {
            throw JoinException(status);
        }
    }
    virtual ~TryJoinT() {
    }
private:
    TryJoinT(const TryJoinT &copy): _joined(*this) {
    }
    TryJoinT(): _joined(*this) {
    }

protected:
    Joined& _joined;
};
typedef TryJoinT<> TryJoin;

} // namespace xos

#endif /// _XOS_BASE_JOINED_HXX_
