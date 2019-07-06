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
///   File: time.cxx
///
/// Author: $author$
///   Date: 5/24/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/microsoft/windows/posix/sys/time.hxx"
#include "xos/logger/Interface.hxx"

namespace xos {
namespace platform {
namespace microsoft {
namespace windows {
namespace posix {
namespace sys {

} /// namespace sys
} /// namespace posix
} /// namespace windows
} /// namespace microsoft
} /// namespace platform
} /// namespace xos

int clock_gettime(clockid_t clockid, struct timespec* timespec) {
    int err = 0;
    timespec->tv_nsec = 0;
    timespec->tv_sec = 0;
    return err;
}
struct tm *localtime_r(const time_t* timep, struct tm* result) {
    struct tm* tm = 0;
    if ((tm = localtime(timep))) {
        *result = *tm;
        return result;
    } else {
        ERR_LOG_ERROR("...failed " << errno << " on localtime()");
    }
    return 0;
}
struct tm *gmtime_r(const time_t* timep, struct tm* result) {
    struct tm* tm = 0;
    if ((tm = gmtime(timep))) {
        *result = *tm;
        return result;
    } else {
        ERR_LOG_ERROR("...failed " << errno << " on gmtime()");
    }
    return 0;
}
