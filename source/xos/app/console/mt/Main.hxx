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
///   Date: 6/15/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_APP_CONSOLE_MT_MAIN_HXX_
#define _XOS_APP_CONSOLE_MT_MAIN_HXX_

#include "xos/app/console/mt/MainOpt.hxx"

namespace xos {
namespace app {
namespace console {
namespace mt {

typedef MainOpt::Implements MainTImplements;
typedef MainOpt MainTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: MainT
///////////////////////////////////////////////////////////////////////
template <class TImplements = MainTImplements, class TExtends = MainTExtends>
class _EXPORT_CLASS MainT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Implements::string_t string_t;
    typedef typename Implements::chars_t chars_t;
    typedef typename Implements::char_t char_t;

    MainT(): _timeout(MSecondsInfinite()) {
    }
    virtual ~MainT() {
    }
private:
    MainT(const MainT& copy): _timeout(0) {
    }

protected:
    virtual mseconds_t SetTimeout(mseconds_t to) {
        _timeout = to;
        return _timeout;
    }
    virtual mseconds_t Timeout() const {
        return _timeout;
    }
    virtual bool InfiniteTimeout(mseconds_t& timeout) const {
        return ((timeout = _timeout) == MSecondsInfinite());
    }
    virtual int OnTimeoutOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        if ((optarg) && (optarg[0])) {
            string_t opt(optarg);
            unsigned to = opt.ToUnsigned();
            SetTimeout(to);
        }
        return err;
    }
    virtual int DerivedRun(int argc, char_t**argv, char_t** env) {
        int err = 0;
        return err;
    }
    virtual int OsRun(int argc, char_t**argv, char_t** env) {
        return DerivedRun(argc, argv, env);
    }
    virtual int PosixRun(int argc, char_t**argv, char_t** env) {
        return DerivedRun(argc, argv, env);
    }
    virtual int OsxRun(int argc, char_t**argv, char_t** env) {
        return DerivedRun(argc, argv, env);
    }
    virtual int WindowsRun(int argc, char_t**argv, char_t** env) {
        return DerivedRun(argc, argv, env);
    }

protected:
    mseconds_t _timeout;
}; /// class _EXPORT_CLASS MainT
typedef MainT<> Main;

} /// namespace mt
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_MT_MAIN_HXX_
