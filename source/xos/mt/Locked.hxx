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
///   File: Locked.hxx
///
/// Author: $author$
///   Date: 6/20/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_LOCKED_HXX_
#define _XOS_MT_LOCKED_HXX_

#include "xos/mt/os/Mutex.hxx"
#include "xos/mt/posix/Mutex.hxx"
#include "xos/mt/apple/osx/Mutex.hxx"
#include "xos/mt/microsoft/windows/Mutex.hxx"

namespace xos {
namespace mt {

///////////////////////////////////////////////////////////////////////
///  Class: LockedT
///////////////////////////////////////////////////////////////////////
template 
<class TExtends = os::Mutex, 
 class TImplements = typename TExtends::Implements>

class _EXPORT_CLASS LockedT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    LockedT(bool isLogged, bool isErrLogged): Extends(isLogged, isErrLogged) {
    }
    LockedT(bool isLogged): Extends(isLogged) {
    }
    LockedT(const LockedT &copy): Extends(copy) {
    }
    LockedT() {
    }
    virtual ~LockedT() {
    }
};
typedef LockedT<> Locked;

} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_LOCKED_HXX_
