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
///   File: Handle.hxx
///
/// Author: $author$
///   Date: 5/13/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_PLATFORM_POSIX_MICROSOFT_WINDOWS_HANDLE_HXX_
#define _XOS_PLATFORM_POSIX_MICROSOFT_WINDOWS_HANDLE_HXX_

#include "xos/platform/posix/microsoft/Windows.hxx"
#include "xos/base/Logged.hxx"

namespace xos {
namespace platform {
namespace posix {
namespace microsoft {
namespace windows {

class _EXPORT_CLASS Handle;
class _EXPORT_CLASS Mutex;
class _EXPORT_CLASS Semaphore;
class _EXPORT_CLASS Thread;

namespace crt {
class _EXPORT_CLASS Thread;
} /// namespace crt

typedef Logged HandleImplements;
typedef extended::Logged HandleExtends;
///////////////////////////////////////////////////////////////////////
///  Class: Handle
///////////////////////////////////////////////////////////////////////
class _EXPORT_CLASS Handle: virtual public HandleImplements, public HandleExtends {
public:
    typedef HandleImplements Implements;
    typedef HandleExtends Extends;

    Handle(bool isLogged, bool isErrLogged): Extends(isLogged, isErrLogged) {
    }
    Handle(bool isLogged): Extends(isLogged) {
    }
    Handle(): Extends(false) {
    }
    virtual ~Handle() {
    }
private:
    Handle(const Handle& copy) {
    }

public:
    virtual BOOL CloseHandle() {
        BOOL success = TRUE;
        return success;
    }
    virtual DWORD WaitForSingleObject(DWORD dwMilliseconds) {
        DWORD dwStatus = WAIT_FAILED;
        return dwStatus;
    }
    virtual DWORD GetLastError(void) {
        DWORD dwLastError = 1;
        return dwLastError;
    }

    virtual Handle* ToHandle() const {
        return ((Handle*)this);
    }
    virtual Mutex* ToMutex() const {
        return ((Mutex*)0);
    }
    virtual Semaphore* ToSemaphore() const {
        return ((Semaphore*)0);
    }
    virtual Thread* ToThread() const {
        return ((Thread*)0);
    }
    virtual crt::Thread* ToCrtThread() const {
        return ((crt::Thread*)0);
    }
}; /// class _EXPORT_CLASS Handle

} /// namespace windows
} /// namespace microsoft
} /// namespace posix
} /// namespace platform
} /// namespace xos

#endif /// _XOS_PLATFORM_POSIX_MICROSOFT_WINDOWS_HANDLE_HXX_
