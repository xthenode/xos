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
///   File: thread.cxx
///
/// Author: $author$
///   Date: 6/4/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/posix/microsoft/windows/thread.hxx"

namespace xos {
namespace platform {
namespace posix {
namespace microsoft {
namespace windows {
namespace crt {

} /// namespace crt
} /// namespace windows
} /// namespace microsoft
} /// namespace posix
} /// namespace platform
} /// namespace xos

#if !defined(WINDOWS)
/// 
/// windows crt threads
/// ...
unsigned* _beginthreadex(
   void *security,
   unsigned stack_size,
   unsigned ( __stdcall *start_address )( void * ),
   void *arglist,
   unsigned initflag,
   unsigned *thrdaddr
) {
    bool initiallySuspended = (CREATE_SUSPENDED == (initflag & CREATE_SUSPENDED));
    try {
        ::xos::platform::posix::microsoft::windows::crt::Thread* thread = 0;

        if ((thread = new ::xos::platform::posix::microsoft::windows::crt::Thread(start_address, arglist, initiallySuspended))) {
            return ((unsigned*)thread);
        }
    } catch (...) {
    }
    return ((unsigned*)((HANDLE)-1));
}
/// ...
/// windows crt threads
/// 
#endif /// !defined(WINDOWS)
