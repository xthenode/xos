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
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/Os.hxx"
#include "xos/platform/Os.c"

#if defined(WINDOWS)  
#include "xos/platform/microsoft/Windows.cxx"
#elif defined(APPLEOSX)  
#include "xos/platform/apple/Osx.cxx"
#elif defined(APPLEIOS)  
#include "xos/platform/apple/Ios.cxx"
#elif defined(ANDROID)
#include "xos/platform/google/Android.cxx"
#elif defined(CHROMEOS)
#include "xos/platform/google/Chromeos.cxx"
#elif defined(LINUX)  
#include "xos/platform/Linux.cxx"
#else /*/ defined(WINDOWS)  /*/
#include "xos/platform/Posix.cxx"
#endif /*/ defined(WINDOWS)  /*/

namespace xos {
namespace platform {


} /// namespace platform
} /// namespace xos

