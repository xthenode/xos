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
///   Date: 6/22/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_OS_SLEEP_HXX_
#define _XOS_MT_OS_SLEEP_HXX_

#include "xos/mt/os/Os.hxx"
#include "xos/base/Base.hxx"

namespace xos {
namespace mt {
namespace os {

///////////////////////////////////////////////////////////////////////
/// Enum: SleepStatus
///////////////////////////////////////////////////////////////////////
typedef int SleepStatus;
enum {
    SleepSuccess = 0,
    SleepFailed,
    SleepBusy,
    SleepInterrupted
};

///////////////////////////////////////////////////////////////////////
///  Class: SleepT
///////////////////////////////////////////////////////////////////////
template <class TImplements = Implement, class TExtends = Extend>
class _EXPORT_CLASS SleepT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    SleepT(mseconds_t milliseconds) {
        Slept(milliseconds);
    }
    virtual ~SleepT() {
    }
private:
    SleepT(const SleepT& copy) {
    }

protected:
    virtual SleepStatus Slept(mseconds_t milliseconds) {
#if !defined(WINDOWS)
        for (useconds_t uSecondsSleep = ::xos::MSecondsUSeconds(10),
             uSeconds = ::xos::MSecondsUSeconds(milliseconds);
             uSecondsSleep <= uSeconds; uSeconds -= uSecondsSleep) {
            usleep(uSecondsSleep);
        }
#else /// !defined(WINDOWS)
#endif /// !defined(WINDOWS)
        return SleepSuccess;
    }
}; /// class _EXPORT_CLASS SleepT
typedef SleepT<> Sleep;

} /// namespace os
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_OS_SLEEP_HXX_
