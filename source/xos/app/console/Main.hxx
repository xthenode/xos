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
///   File: Main.hxx
///
/// Author: $author$
///   Date: 6/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_APP_CONSOLE_MAIN_HXX_
#define _XOS_APP_CONSOLE_MAIN_HXX_

#include "xos/app/console/MainOpt.hxx"

#define __LOCATION__ \
    __FILE__, "[", UnsignedToString(__LINE__).Chars(), "] "

namespace xos {
namespace app {
namespace console {

typedef MainOpt::Implements MainTImplements;
typedef MainOpt MainTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: MainT
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = MainOpt::Implements, class TExtends = MainOpt>

class _EXPORT_CLASS MainT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Implements::string_t string_t;
    typedef typename Implements::chars_t chars_t;
    typedef typename Implements::char_t char_t;

    MainT() {
    }
    virtual ~MainT() {
    }
private:
    MainT(const MainT& copy) {
    }

protected:
}; /// class _EXPORT_CLASS MainT
typedef MainT<> Main;

} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_MAIN_HXX_
