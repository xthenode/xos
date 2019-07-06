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
///   Date: 5/6/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_CONSOLE_LIB_VERSION_MAIN_HXX_
#define _XOS_CONSOLE_LIB_VERSION_MAIN_HXX_

#include "xos/console/getopt/Main.hxx"
#include "xos/lib/xene/Version.hxx"

namespace xos {
namespace console {
namespace lib {
namespace version {

typedef ::xos::console::getopt::Main::Implements MainTImplements;
typedef ::xos::console::getopt::Main MainTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: MainT
///////////////////////////////////////////////////////////////////////
template 
<class TVersion = ::xos::lib::xene::Version, 
 class TImplements = MainTImplements, class TExtends = MainTExtends>

class _EXPORT_CLASS MainT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef TVersion version_t;
    typedef typename Extends::char_t char_t;

    MainT() {
    }
    virtual ~MainT() {
    }
private:
    MainT(const MainT &copy) {
    }

protected:
    virtual int Run(int argc, char_t**argv, char_t** env) {
        int err = this->VersionRun(argc, argv, env);
        return err;
    }
    virtual int VersionRun(int argc, char_t**argv, char_t** env) {
        const ::xos::lib::Version& version = version_t::Which();
        int err = 0;
        this->OutLLn(version.Name().Chars(), " version = ", version.ToString().Chars(), NULL);
        return err;
    }
};

} /// namespace version
} /// namespace lib
} /// namespace console
} /// namespace xos

#endif /// _XOS_CONSOLE_LIB_VERSION_MAIN_HXX_
