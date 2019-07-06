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
///   File: io.cxx
///
/// Author: $author$
///   Date: 6/3/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/posix/microsoft/windows/io.hxx"

namespace xos {
namespace platform {
namespace posix {
namespace microsoft {
namespace windows {

} /// namespace windows
} /// namespace microsoft
} /// namespace posix
} /// namespace platform
} /// namespace xos

#if !defined(WINDOWS)
///
/// windows io
/// ...
int _fileno(
   FILE *stream
) {
    return ::fileno(stream);
}
int _setmode (
   int fd,
   int mode
) {
    if (0 <= (fd)) {
        return 0;
    }
    return -1;
}
/// ...
/// windows io
/// 
#endif /// !defined(WINDOWS)
