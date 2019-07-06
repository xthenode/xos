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
///   File: Windows.hxx
///
/// Author: $author$
///   Date: 5/3/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_PLATFORM_MICROSOFT_WINDOWS_HXX_
#define _XOS_PLATFORM_MICROSOFT_WINDOWS_HXX_

#include "xos/platform/microsoft/Windows.h"

#if (_MSC_VER > MSC_VER_6)
#if !defined(__interface__)
#if defined(interface)
#define interface_define interface
#undef interface
#endif /// defined(interface)
#define interface __interface__
#endif /// !defined(__interface__)
#endif /// (_MSC_VER > MSC_VER_6)

namespace xos {
namespace platform {

} /// namespace platform
} /// namespace xos 

#endif /// _XOS_PLATFORM_MICROSOFT_WINDOWS_HXX_
