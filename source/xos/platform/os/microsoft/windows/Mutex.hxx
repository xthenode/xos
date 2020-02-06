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
///   File: Mutex.hxx
///
/// Author: $author$
///   Date: 6/21/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_PLATFORM_OS_MICROSOFT_WINDOWS_MUTEX_HXX_
#define _XOS_PLATFORM_OS_MICROSOFT_WINDOWS_MUTEX_HXX_

#include "xos/platform/os/microsoft/windows/Mutex.h"
#include "xos/platform/os/microsoft/windows/Handle.hxx"
#include "xos/mt/os/Mutex.hxx"

namespace xos {
namespace platform {
namespace os {
namespace microsoft {
namespace windows {

typedef Handle::Implements MutexImplements;
typedef Handle MutexExtends;
///////////////////////////////////////////////////////////////////////
///  Class: Mutex
///////////////////////////////////////////////////////////////////////
class _EXPORTED_ Mutex: virtual public MutexImplements, public MutexExtends {
public:
    typedef MutexImplements Implements;
    typedef MutexExtends Extends;

    Mutex(BOOL bInitialOwner): _mutex(((::xos::mt::os::Mutex::Attached)0), false, false) {
        if (!(_mutex.Create(bInitialOwner))) {
            throw CreateException(CreateFailed);
        }
    }
    Mutex(const Mutex& copy): _mutex(copy._mutex) {
    }
    Mutex(): _mutex(((::xos::mt::os::Mutex::Attached)0), false, false) {
        if (!(_mutex.Create())) {
            throw CreateException(CreateFailed);
        }
    }
    virtual ~Mutex() {
    }

    virtual BOOL CloseHandle() {
        BOOL success = (_mutex.Destroy())?(TRUE):(FALSE);
        return success;
    }
    virtual DWORD WaitForSingleObject(DWORD dwMilliseconds) {
        DWORD dwStatus = WAIT_FAILED;
        ::xos::LockStatus status = ::xos::LockFailed;
        if (INFINITE != (dwMilliseconds)) {
            if (0 < (dwMilliseconds)) {
                status = _mutex.TimedLock(dwMilliseconds);
            } else {
                status = _mutex.TryLock();
            }
        } else {
            status = _mutex.UntimedLock();
        }
        switch (status) {
        case ::xos::LockSuccess:
            dwStatus = WAIT_OBJECT_0;
            break;
        case ::xos::LockBusy:
            dwStatus = WAIT_TIMEOUT;
            break;
        case ::xos::LockInterrupted:
            dwStatus = WAIT_ABANDONED;
            break;
        }
        return dwStatus;
    }
    virtual DWORD GetLastError(void) {
        DWORD dwLastError = 1;
        return dwLastError;
    }
    virtual BOOL ReleaseMutex() {
        BOOL success = FALSE;
        if ((_mutex.Unlock())) {
            success = TRUE;
        }
        return success;
    }
    virtual Mutex* ToMutex() const {
        return ((Mutex*)this);
    }

protected:
    ::xos::mt::os::Mutex _mutex;
}; /// class _EXPORTED_ Mutex

} /// namespace windows
} /// namespace microsoft
} /// namespace os
} /// namespace platform
} /// namespace xos

#endif /// _XOS_PLATFORM_OS_MICROSOFT_WINDOWS_MUTEX_HXX_
