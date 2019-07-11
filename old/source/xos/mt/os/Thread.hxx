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
///   File: Thread.hxx
///
/// Author: $author$
///   Date: 6/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_OS_THREAD_HXX_
#define _XOS_MT_OS_THREAD_HXX_

#include "xos/mt/os/Os.hxx"
#include "xos/mt/Thread.hxx"

#if defined(WINDOWS)
#include "xos/mt/microsoft/windows/crt/Thread.hxx"
#elif defined(MACOSX)
#include "xos/mt/apple/osx/Thread.hxx"
#elif defined(LINUX)
#include "xos/mt/linux/Thread.hxx"
#else /// defined(WINDOWS)
#include "xos/mt/posix/Thread.hxx"
#endif /// defined(WINDOWS)

namespace xos {
namespace mt {
namespace os {

#if defined(WINDOWS)
typedef os::crt::Thread Thread;
#else /// defined(WINDOWS)
typedef os::Thread Thread;
#endif /// defined(WINDOWS)

} /// namespace os
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_OS_THREAD_HXX_
