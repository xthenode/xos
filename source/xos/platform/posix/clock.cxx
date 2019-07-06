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
///   File: clock.cxx
///
/// Author: $author$
///   Date: 6/3/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/posix/clock.hxx"

namespace xos {
namespace platform {
namespace posix {

} /// namespace posix
} /// namespace platform
} /// namespace xos

#if !defined(WINDOWS) && !defined(HAS_POSIX_TIMEOUTS)
///
/// clock
/// ...
#if defined(HAS_CLOCK_GETTIME)
int clock_gettime(clockid_t clk_id, struct timespec *res) {
    if ((CLOCK_REALTIME == clk_id) && (res)) {
        memset(res, 0, sizeof(struct timespec));
        return 0;
    }
    return EINVAL;
}
#endif /// defined(HAS_CLOCK_GETTIME)
/// ...
/// clock
///
#endif /// !defined(WINDOWS) && !defined(HAS_POSIX_TIMEOUTS)
