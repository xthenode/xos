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
///   File: MainOpt.hxx
///
/// Author: $author$
///   Date: 6/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_APP_CONSOLE_OS_MAINOPT_HXX_
#define _XOS_APP_CONSOLE_OS_MAINOPT_HXX_

#include "xos/app/console/Main.hxx"

#define XOS_APP_CONSOLE_OS_MAIN_OS_OPT "os"
#define XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_REQUIRED MAIN_OPT_ARGUMENT_REQUIRED
#define XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_RESULT 0
#define XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_WINDOWS_C "w" 
#define XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_WINDOWS_S "windows" 
#define XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_MACOSX_C "x" 
#define XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_MACOSX_S "osx" 
#define XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_LINUX_C "l" 
#define XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_LINUX_S "linux" 
#define XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_POSIX_C "p" 
#define XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_POSIX_S "posix" 
#define XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG \
    "{ " "(" XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_WINDOWS_C ")" XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_WINDOWS_S \
    " | " "(" XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_MACOSX_C ")" XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_MACOSX_S \
    " | " "(" XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_LINUX_C ")" XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_LINUX_S \
    " | " "(" XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_POSIX_C ")" XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_POSIX_S \
    " }" 
#define XOS_APP_CONSOLE_OS_MAIN_OS_OPTUSE ""
#define XOS_APP_CONSOLE_OS_MAIN_OS_OPTVAL_S "o:"
#define XOS_APP_CONSOLE_OS_MAIN_OS_OPTVAL_C 'o'
#define XOS_APP_CONSOLE_OS_MAIN_OS_OPTION \
   {XOS_APP_CONSOLE_OS_MAIN_OS_OPT, \
    XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_REQUIRED, \
    XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_RESULT, \
    XOS_APP_CONSOLE_OS_MAIN_OS_OPTVAL_C}, \

#define XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPT "runtime-library"
#define XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_REQUIRED MAIN_OPT_ARGUMENT_REQUIRED
#define XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_RESULT 0
#define XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_NATIVE_C "n" 
#define XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_NATIVE_S "native" 
#define XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_CLIB_C "c" 
#define XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_CLIB_S "clib" 
#define XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG \
    "{ " "(" XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_NATIVE_C ")" XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_NATIVE_S \
    " | " "(" XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_CLIB_C ")" XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_CLIB_S \
    " }" 
#define XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTUSE ""
#define XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTVAL_S "r:"
#define XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTVAL_C 'r'
#define XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTION \
   {XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPT, \
    XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_REQUIRED, \
    XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_RESULT, \
    XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTVAL_C}, \

#define XOS_APP_CONSOLE_OS_MAIN_OPTIONS_CHARS \
   XOS_APP_CONSOLE_OS_MAIN_OS_OPTVAL_S \
   XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTVAL_S \
   XOS_CONSOLE_MAIN_OPTIONS_CHARS

#define XOS_APP_CONSOLE_OS_MAIN_OPTIONS_OPTIONS \
   XOS_APP_CONSOLE_OS_MAIN_OS_OPTION \
   XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTION \
   XOS_CONSOLE_MAIN_OPTIONS_OPTIONS

#define XOS_APP_CONSOLE_OS_MAIN_ARGS 0
#define XOS_APP_CONSOLE_OS_MAIN_ARGV

namespace xos {
namespace app {
namespace console {
namespace os {

///////////////////////////////////////////////////////////////////////
///  Class: MainOptT
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = Main::Implements, class TExtends = Main>

class _EXPORT_CLASS MainOptT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Implements::string_t string_t;
    typedef typename Implements::chars_t chars_t;
    typedef typename Implements::char_t char_t;

    MainOptT() {
    }
    virtual ~MainOptT() {
    }
private:
    MainOptT(const MainOptT& copy) {
    }

protected:
    virtual int OnWindowsOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        return err;
    }
    virtual int OnOsxOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        return err;
    }
    virtual int OnLinuxOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        return err;
    }
    virtual int OnPosixOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        return err;
    }
    virtual int OnOsOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        if ((optarg) && (optarg[0])) {
            string_t arg(optarg);
            if ((!arg.compare(XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_WINDOWS_C)) 
                || (!arg.compare(XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_WINDOWS_S))) {
                err = OnWindowsOption
                (optval, optarg, optname, optind, argc, argv, env);
            } else {
                if ((!arg.compare(XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_MACOSX_C)) 
                    || (!arg.compare(XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_MACOSX_S))) {
                    err = OnOsxOption
                    (optval, optarg, optname, optind, argc, argv, env);
                } else {
                    if ((!arg.compare(XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_LINUX_C)) 
                        || (!arg.compare(XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_LINUX_S))) {
                        err = OnLinuxOption
                        (optval, optarg, optname, optind, argc, argv, env);
                    } else {
                        if ((!arg.compare(XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_POSIX_C)) 
                            || (!arg.compare(XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG_POSIX_S))) {
                            err = OnPosixOption
                            (optval, optarg, optname, optind, argc, argv, env);
                        } else {
                            err = Extends::OnInvalidOptionArg
                            (optval, optarg, optname, optind, argc, argv, env);
                        }
                    }
                }
            }
        }
        return err;
    }
    virtual int OnNativeRuntimeOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        return err;
    }
    virtual int OnClibRuntimeOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        return err;
    }
    virtual int OnRuntimeOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        if ((optarg) && (optarg[0])) {
            string_t arg(optarg);
            if ((!arg.compare(XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_NATIVE_C)) 
                || (!arg.compare(XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_NATIVE_S))) {
                err = OnNativeRuntimeOption
                (optval, optarg, optname, optind, argc, argv, env);
            } else {
                if ((!arg.compare(XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_CLIB_C)) 
                    || (!arg.compare(XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG_CLIB_S))) {
                    err = OnClibRuntimeOption
                    (optval, optarg, optname, optind, argc, argv, env);
                } else {
                    err = Extends::OnInvalidOptionArg
                    (optval, optarg, optname, optind, argc, argv, env);
                }
            }
        }
        return err;
    }
    virtual int OnOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        switch(optval) {
        case XOS_APP_CONSOLE_OS_MAIN_OS_OPTVAL_C:
            err = OnOsOption
            (optval, optarg, optname, optind, argc, argv, env);
            break;
        case XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTVAL_C:
            err = OnRuntimeOption
            (optval, optarg, optname, optind, argc, argv, env);
            break;
        default:
            err = Extends::OnOption
            (optval, optarg, optname, optind, argc, argv, env);
        }
        return err;
    }
    virtual const char_t* OptionUsage
    (const char_t*& optarg, const struct option* longopt) {
        const char_t* chars = "";
        switch(longopt->val) {
        case XOS_APP_CONSOLE_OS_MAIN_OS_OPTVAL_C:
            optarg = XOS_APP_CONSOLE_OS_MAIN_OS_OPTARG;
            chars = XOS_APP_CONSOLE_OS_MAIN_OS_OPTUSE;
            break;
        case XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTVAL_C:
            optarg = XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTARG;
            chars = XOS_APP_CONSOLE_OS_MAIN_RUNTIME_OPTUSE;
            break;
        default:
            chars = Extends::OptionUsage(optarg, longopt);
        }
        return chars;
    }
    virtual const char_t* Options(const struct option*& longopts) {
        int err = 0;
        static const char_t* chars = XOS_APP_CONSOLE_OS_MAIN_OPTIONS_CHARS;
        static struct option optstruct[]= {
            XOS_APP_CONSOLE_OS_MAIN_OPTIONS_OPTIONS
            {0, 0, 0, 0}};
        longopts = optstruct;
        return chars;
    }
    virtual const char_t* Arguments(const char_t**& argv) {
        static const char_t* _args = XOS_APP_CONSOLE_OS_MAIN_ARGS;
        static const char_t* _argv[] = {
            XOS_APP_CONSOLE_OS_MAIN_ARGV
            0};
        argv = _argv;
        return _args;
    }
}; /// class _EXPORT_CLASS MainOptT
typedef MainOptT<> MainOpt;

} /// namespace os
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_OS_MAINOPT_HXX_
