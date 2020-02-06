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
///   File: Joined.hxx
///
/// Author: $author$
///   Date: 7/17/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_JOINED_HXX_
#define _XOS_MT_JOINED_HXX_

#include "xos/mt/os/Sleep.hxx"
#include "xos/mt/os/Thread.hxx"
#include "xos/mt/linux/Thread.hxx"
#include "xos/mt/posix/Thread.hxx"
#include "xos/mt/apple/osx/Thread.hxx"
#include "xos/mt/microsoft/windows/Thread.hxx"
#include "xos/mt/microsoft/windows/crt/Thread.hxx"

namespace xos {
namespace mt {

///////////////////////////////////////////////////////////////////////
///  Class: JoinedT
///////////////////////////////////////////////////////////////////////
template 
<class TExtends = os::Thread, 
 class TImplements = typename TExtends::Implements>

class _EXPORTED_ JoinedT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef typename Implements::Ran Ran;

    JoinedT(bool isLogged, bool isErrLogged): Extends(isLogged, isErrLogged) {
    }
    JoinedT(bool isLogged): Extends(isLogged) {
    }
    JoinedT(const JoinedT &copy): Extends(copy) {
    }
    JoinedT(Ran& ran): Extends(ran) {
    }
    JoinedT() {
    }
    virtual ~JoinedT() {
    }
};
typedef JoinedT<> Joined;

} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_JOINED_HXX_
