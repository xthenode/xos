/*/
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
///   File: Os.c
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
/*/
#include "xos/platform/Os.h"

#if defined(WINDOWS)  
#include "xos/platform/microsoft/Windows.c"
#elif defined(APPLEOSX)  
#include "xos/platform/apple/Osx.c"
#elif defined(APPLEIOS)  
#include "xos/platform/apple/Ios.c"
#elif defined(ANDROID)
#include "xos/platform/google/Android.c"
#elif defined(CHROMEOS)
#include "xos/platform/google/Chromeos.c"
#elif defined(LINUX)  
#include "xos/platform/Linux.c"
#else /*/ defined(WINDOWS)  /*/
#include "xos/platform/Posix.c"
#endif /*/ defined(WINDOWS)  /*/
