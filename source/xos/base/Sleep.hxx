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
///   File: Sleep.hxx
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_BASE_SLEEP_HXX_
#define _XOS_BASE_SLEEP_HXX_

#include "xos/base/Exception.hxx"

namespace xos {

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
enum SleepStatus {
    WakeSuccess,
    SleepSuccess = WakeSuccess,

    SleepFailed,
    SleepBusy,
    SleepInterrupted,
    SleepInvalid,

    WakeFailed,
    WakeBusy,
    WakeInterrupted,
    WakeInvalid
};

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
inline const char* SleepStatusToChars(SleepStatus status) {
    switch (status) {
    case SleepSuccess: return "SleepSuccess";
    case SleepFailed: return "SleepFailed";
    case SleepBusy: return "SleepBusy";
    case SleepInterrupted: return "SleepInterrupted";
    case SleepInvalid: return "SleepInvalid";

    case WakeFailed: return "WakeFailed";
    case WakeBusy: return "WakeBusy";
    case WakeInterrupted: return "WakeInterrupted";
    case WakeInvalid: return "WakeInvalid";
    }
    return "Unknown";
}

typedef ImplementBase SleepExceptionTImplements;
typedef Base SleepExceptionTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: SleepExceptionT
///////////////////////////////////////////////////////////////////////
template
<class TImplements = SleepExceptionTImplements,
 class TExtends = SleepExceptionTExtends>

class _EXPORTED_ SleepExceptionT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    SleepExceptionT(SleepStatus status): m_status(status) {}
    virtual ~SleepExceptionT() {}
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual SleepStatus Status() const {
        return m_status;
    }
    virtual String StatusToString() const {
        String to(StatusToChars());
        return to;
    }
    virtual const char* StatusToChars() const {
        return SleepStatusToChars(Status());
    }
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    SleepStatus m_status;
};
typedef SleepExceptionT<> SleepException;

typedef ImplementBase SleeperTImplements;
///////////////////////////////////////////////////////////////////////
///  Class: SleeperT
///////////////////////////////////////////////////////////////////////
template <class TImplements = SleeperTImplements>
class _EXPORTED_ SleeperT: virtual public TImplements {
public:
    typedef TImplements Implements;
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual SleepStatus SleepSeconds(seconds_t seconds) {
        return SleepFailed;
    }
    virtual SleepStatus SleepMilliseconds(mseconds_t milliseconds) {
        return SleepFailed;
    }
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
};
typedef SleeperT<> Sleeper;

} /// namespace xos

#endif /// _XOS_BASE_SLEEP_HXX_
