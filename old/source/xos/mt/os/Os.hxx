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
///   File: Os.hxx
///
/// Author: $author$
///   Date: 5/7/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_OS_OS_HXX_
#define _XOS_MT_OS_OS_HXX_

#include "xos/platform/Os.hxx"

#if defined(WINDOWS)
#include "xos/mt/microsoft/windows/Os.hxx"
#elif defined(APPLEOSX)
#include "xos/mt/apple/osx/Os.hxx"
#elif defined(APPLEIOS)
#include "xos/mt/apple/ios/Os.hxx"
#elif defined(ANDROID)
#include "xos/mt/google/android/Os.hxx"
#elif defined(CHROMEOS)
#include "xos/mt/google/chromeos/Os.hxx"
#elif defined(LINUX)
#include "xos/mt/linux/Os.hxx"
#else /// defined(WINDOWS)
#include "xos/mt/posix/Os.hxx"
#endif /// defined(WINDOWS)

namespace xos {
namespace mt {
namespace microsoft { namespace windows { namespace crt {} } }
namespace apple { namespace osx {} namespace ios {} namespace mach {} }
namespace google { namespace android {} namespace chromeos {} }
namespace linux {}
namespace posix {}
namespace os {

#if defined(WINDOWS)
namespace os = microsoft::windows;
#elif defined(APPLEOSX)
namespace os = apple::osx;
#elif defined(APPLEIOS)
namespace os = apple::ios;
#elif defined(ANDROID)
namespace os = google::android;
#elif defined(CHROMEOS)
namespace os = google::chromeos;
#elif defined(LINUX)
namespace os = linux;
#else /// defined(WINDOWS)
namespace os = posix;
#endif /// defined(WINDOWS)

} /// namespace os
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_OS_OS_HXX_
