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
///   File: Handle.cxx
///
/// Author: $author$
///   Date: 7/27/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/os/microsoft/windows/Handle.hxx"

namespace xos {
namespace platform {
namespace os {
namespace microsoft {
namespace windows {

} /// namespace windows
} /// namespace microsoft
} /// namespace os
} /// namespace platform
} /// namespace xos

#if !defined(WINDOWS)
/// 
/// windows handles
/// ...
BOOL WINAPI CloseHandle(
  _In_ HANDLE hObject
) {
    ::xos::platform::os::microsoft::windows::Handle* handle = 0;
    if ((handle = ((::xos::platform::os::microsoft::windows::Handle*)hObject))) {
        BOOL success = handle->CloseHandle();
        delete handle;
        return success;
    }
    return FALSE;
}
DWORD WINAPI WaitForSingleObject(
  _In_ HANDLE hHandle,
  _In_ DWORD  dwMilliseconds
) {
    ::xos::platform::os::microsoft::windows::Handle* handle = 0;
    if ((handle = ((::xos::platform::os::microsoft::windows::Handle*)hHandle))) {
        DWORD dwStatus = handle->WaitForSingleObject(dwMilliseconds);
        return dwStatus;
    }
    return WAIT_FAILED;
}
/// ...
/// windows handles
/// 
#endif /// !defined(WINDOWS)
