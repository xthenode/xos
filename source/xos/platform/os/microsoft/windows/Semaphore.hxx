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
///   File: Semaphore.hxx
///
/// Author: $author$
///   Date: 6/24/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_PLATFORM_OS_MICROSOFT_WINDOWS_SEMAPHORE_HXX_
#define _XOS_PLATFORM_OS_MICROSOFT_WINDOWS_SEMAPHORE_HXX_

#include "xos/mt/os/Semaphore.hxx"
#include "xos/platform/os/microsoft/windows/Handle.hxx"

namespace xos {
namespace platform {
namespace os {
namespace microsoft {
namespace windows {

typedef Handle::Implements SemaphoreImplements;
typedef Handle SemaphoreExtends;
///////////////////////////////////////////////////////////////////////
///  Class: Semaphore
///////////////////////////////////////////////////////////////////////
class _EXPORT_CLASS Semaphore: virtual public SemaphoreImplements, public SemaphoreExtends {
public:
    typedef Implement Implements;
    typedef Extend Extends;

    Semaphore(LONG lInitialCount, LONG lMaximumCount)
    : _semaphore(((::xos::mt::os::Semaphore::Attached)0), false, false) {
        if (!(_semaphore.Create(lInitialCount))) {
            throw CreateException(CreateFailed);
        }
    }
    Semaphore(const Semaphore& copy): _semaphore(copy._semaphore) {
    }
    Semaphore()
    : _semaphore(((::xos::mt::os::Semaphore::Attached)0), false, false) {
        if (!(_semaphore.Create())) {
            throw CreateException(CreateFailed);
        }
    }
    virtual ~Semaphore() {
    }

    virtual BOOL CloseHandle() {
        BOOL success = (_semaphore.Destroy())?(TRUE):(FALSE);
        return success;
    }
    virtual DWORD WaitForSingleObject(DWORD dwMilliseconds) {
        DWORD dwStatus = WAIT_FAILED;
        ::xos::AcquireStatus status = ::xos::AcquireFailed;

        if (INFINITE != (dwMilliseconds)) {
            if (0 < (dwMilliseconds)) {
                status = _semaphore.TimedAcquire(dwMilliseconds);
            } else {
                status = _semaphore.TryAcquire();
            }
        } else {
            status = _semaphore.UntimedAcquire();
        }
        switch (status) {
        case ::xos::AcquireSuccess:
            dwStatus = WAIT_OBJECT_0;
            break;
        case ::xos::AcquireBusy:
            dwStatus = WAIT_TIMEOUT;
            break;
        case ::xos::AcquireInterrupted:
            dwStatus = WAIT_ABANDONED;
            break;
        }
        return dwStatus;
    }
    virtual DWORD GetLastError(void) {
        DWORD dwLastError = 1;
        return dwLastError;
    }
    virtual BOOL ReleaseSemaphore(LONG lReleaseCount, LPLONG lpPreviousCount) {
        BOOL success = FALSE;
        if ((0 < lReleaseCount) && (2 > lReleaseCount)) {
            if ((_semaphore.Release())) {
                success = TRUE;
            }
        }
        return success;
    }
    virtual Semaphore* ToSemaphore() const {
        return ((Semaphore*)this);
    }
    
protected:
    ::xos::mt::os::Semaphore _semaphore;
}; /// class _EXPORT_CLASS Class

} /// namespace windows
} /// namespace microsoft
} /// namespace os
} /// namespace platform
} /// namespace xos

#if !defined(WINDOWS)
///
/// windows semaphores
/// ...
HANDLE WINAPI CreateSemaphore(
  _In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
  _In_     LONG                  lInitialCount,
  _In_     LONG                  lMaximumCount,
  _In_opt_ LPCTSTR               lpName
) {
    try {
        ::xos::platform::os::microsoft::windows::Semaphore* semaphore = 0;

        if ((semaphore = new ::xos::platform::os::microsoft::windows::Semaphore(lInitialCount, lMaximumCount))) {
            return semaphore;
        }
    } catch (...) {
    }
    return NULL_HANDLE;
}
BOOL WINAPI ReleaseSemaphore(
  _In_      HANDLE hSemaphore,
  _In_      LONG   lReleaseCount,
  _Out_opt_ LPLONG lpPreviousCount
) {
    ::xos::platform::os::microsoft::windows::Handle* handle = 0;

    if ((handle = ((::xos::platform::os::microsoft::windows::Handle*)hSemaphore))) {
        ::xos::platform::os::microsoft::windows::Semaphore* semaphore = 0;

        if ((semaphore = handle->ToSemaphore())) {
            BOOL success = FALSE;

            if ((success = semaphore->ReleaseSemaphore(lReleaseCount, lpPreviousCount))) {
                return success;
            }
        }
    }
    return FALSE;
}
/// ...
/// windows semaphores
/// 
#endif /// !defined(WINDOWS)

#endif /// _XOS_PLATFORM_OS_MICROSOFT_WINDOWS_SEMAPHORE_HXX_
