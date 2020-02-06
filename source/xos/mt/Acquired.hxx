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
///   File: Acquired.hxx
///
/// Author: $author$
///   Date: 7/11/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_ACQUIRED_HXX_
#define _XOS_MT_ACQUIRED_HXX_

#include "xos/mt/os/Semaphore.hxx"
#include "xos/mt/posix/Semaphore.hxx"
#include "xos/mt/apple/mach/Semaphore.hxx"
#include "xos/mt/microsoft/windows/Semaphore.hxx"

namespace xos {
namespace mt {

///////////////////////////////////////////////////////////////////////
///  Class: AcquiredT
///////////////////////////////////////////////////////////////////////
template 
<class TExtends = os::Semaphore, 
 class TImplements = typename TExtends::Implements>

class _EXPORTED_ AcquiredT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    AcquiredT(bool isLogged, bool isErrLogged): Extends(isLogged, isErrLogged) {
    }
    AcquiredT(bool isLogged): Extends(isLogged) {
    }
    AcquiredT(const AcquiredT &copy): Extends(copy) {
    }
    AcquiredT() {
    }
    virtual ~AcquiredT() {
    }
}; /// class _EXPORTED_ AcquiredT

} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_ACQUIRED_HXX_
