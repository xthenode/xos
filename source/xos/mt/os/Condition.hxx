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
///   File: Condition.hxx
///
/// Author: $author$
///   Date: 7/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_OS_CONDITION_HXX_
#define _XOS_MT_OS_CONDITION_HXX_

#include "xos/mt/os/Mutex.hxx"

#if defined(WINDOWS)
#include "xos/mt/microsoft/windows/Condition.hxx"
#elif defined(MACOSX)
#include "xos/mt/apple/osx/Condition.hxx"
#elif defined(APPIOS)
#include "xos/mt/apple/ios/Condition.hxx"
#elif defined(ANDROID)
#include "xos/mt/google/android/Condition.hxx"
#elif defined(CHROMEOS)
#include "xos/mt/google/chromeos/Condition.hxx"
#elif defined(LINUX)
#include "xos/mt/linux/Condition.hxx"
#else /// defined(WINDOWS)
#include "xos/mt/posix/Condition.hxx"
#endif /// defined(WINDOWS)

namespace xos {
namespace mt {
namespace os {

typedef os::Condition Condition;

} /// namespace os
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_OS_CONDITION_HXX_
