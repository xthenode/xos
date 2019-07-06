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
///   File: Os.cxx
///
/// Author: $author$
///   Date: 7/3/2019
///////////////////////////////////////////////////////////////////////
#include "xos/os/Os.hxx"

#if defined(WINDOWS)
#include "xos/os/microsoft/windows/Os.cxx"
#elif defined(MACOSX)
#include "xos/os/apple/osx/Os.cxx"
#elif defined(APPIOS)
#include "xos/os/apple/ios/Os.cxx"
#elif defined(ANDROID)
#include "xos/os/google/android/Os.cxx"
#elif defined(CHROMEOS)
#include "xos/os/google/chromeos/Os.cxx"
#elif defined(LINUX)
#include "xos/os/linux/Os.cxx"
#else /// defined(WINDOWS)
#include "xos/os/posix/Os.cxx"
#endif /// defined(WINDOWS)

namespace xos {
namespace os {

} /// namespace os
} /// namespace xos
