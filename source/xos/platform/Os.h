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
///   File: Os.h
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
/*/
#ifndef _XOS_PLATFORM_OS_H_
#define _XOS_PLATFORM_OS_H_

#include "xos/platform/Build.h"

#if defined(WINDOWS)  
#include "xos/platform/microsoft/Windows.h"
#elif defined(APPLEOSX)  
#include "xos/platform/apple/Osx.h"
#elif defined(APPLEIOS)  
#include "xos/platform/apple/Ios.h"
#elif defined(ANDROID)
#include "xos/platform/google/Android.h"
#elif defined(CHROMEOS)
#include "xos/platform/google/Chromeos.h"
#elif defined(LINUX)  
#include "xos/platform/Linux.h"
#else /*/ defined(WINDOWS)  /*/
#include "xos/platform/Posix.h"
#endif /*/ defined(WINDOWS)  /*/

#include "xos/platform/Configure.h"

#endif /*/ _XOS_PLATFORM_OS_H_ /*/
