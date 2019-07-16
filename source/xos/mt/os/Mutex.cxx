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
///   File: Mutex.cxx
///
/// Author: $author$
///   Date: 6/21/2019
///////////////////////////////////////////////////////////////////////
#include "xos/mt/os/Mutex.hxx"

#if defined(WINDOWS)
#include "xos/mt/microsoft/windows/Mutex.cxx"
#elif defined(MACOSX)
#include "xos/mt/apple/osx/Mutex.cxx"
#elif defined(APPIOS)
#include "xos/mt/apple/ios/Mutex.cxx"
#elif defined(ANDROID)
#include "xos/mt/google/android/Mutex.cxx"
#elif defined(CHROMEOS)
#include "xos/mt/google/chromeos/Mutex.cxx"
#elif defined(LINUX)
#include "xos/mt/linux/Mutex.cxx"
#else /// defined(WINDOWS)
#include "xos/mt/posix/Mutex.cxx"
#endif /// defined(WINDOWS)

namespace xos {
namespace mt {
namespace os {

} /// namespace os
} /// namespace mt
} /// namespace xos
