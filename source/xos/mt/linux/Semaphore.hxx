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
///   File: Semaphore.hxx
///
/// Author: $author$
///   Date: 7/12/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_LINUX_SEMAPHORE_HXX_
#define _XOS_MT_LINUX_SEMAPHORE_HXX_

#include "xos/mt/posix/Semaphore.hxx"

namespace xos {
namespace mt {
namespace linux {

typedef posix::Semaphore Semaphore;

} /// namespace linux
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_LINUX_SEMAPHORE_HXX_
