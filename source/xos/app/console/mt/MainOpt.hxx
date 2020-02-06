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
///   Date: 6/15/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_APP_CONSOLE_MT_MAINOPT_HXX_
#define _XOS_APP_CONSOLE_MT_MAINOPT_HXX_

#include "xos/mt/os/Os.hxx"
#include "xos/app/console/os/Main.hxx"

#define XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPT "sleep"
#define XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTARG_REQUIRED MAIN_OPT_ARGUMENT_REQUIRED
#define XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTARG_RESULT 0
#define XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTARG "milliseconds"
#define XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTUSE "Sleep Timeout"
#define XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTVAL_S "s:"
#define XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTVAL_C 's'
#define XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTION \
   {XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPT, \
    XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTARG_REQUIRED, \
    XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTARG_RESULT, \
    XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTVAL_C}, \

#define XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPT "timeout"
#define XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTARG_REQUIRED MAIN_OPT_ARGUMENT_REQUIRED
#define XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTARG_RESULT 0
#define XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTARG "milliseconds"
#define XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTUSE "Wait Timeout"
#define XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTVAL_S "t:"
#define XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTVAL_C 't'
#define XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTION \
   {XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPT, \
    XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTARG_REQUIRED, \
    XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTARG_RESULT, \
    XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTVAL_C}, \

#define XOS_APP_CONSOLE_MT_MAIN_OPTIONS_CHARS \
   XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTVAL_S \
   XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTVAL_S \
   XOS_APP_CONSOLE_OS_MAIN_OPTIONS_CHARS

#define XOS_APP_CONSOLE_MT_MAIN_OPTIONS_OPTIONS \
   XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTION \
   XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTION \
   XOS_APP_CONSOLE_OS_MAIN_OPTIONS_OPTIONS

#define XOS_APP_CONSOLE_MT_MAIN_ARGS 0
#define XOS_APP_CONSOLE_MT_MAIN_ARGV

namespace xos {
namespace app {
namespace console {
namespace mt {

///////////////////////////////////////////////////////////////////////
///  Class: MainOptT
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = os::Main::Implements, class TExtends = os::Main>

class _EXPORTED_ MainOptT: virtual public TImplements, public TExtends {
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
    virtual int OnSleepOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        return err;
    }
    virtual int OnTimeoutOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        return err;
    }
    virtual int OnOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        switch(optval) {
        case XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTVAL_C:
            err = OnSleepOption
            (optval, optarg, optname, optind, argc, argv, env);
            break;
        case XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTVAL_C:
            err = OnTimeoutOption
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
        case XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTVAL_C:
            optarg = XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTARG;
            chars = XOS_APP_CONSOLE_MT_MAIN_SLEEP_OPTUSE;
            break;
        case XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTVAL_C:
            optarg = XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTARG;
            chars = XOS_APP_CONSOLE_MT_MAIN_TIMEOUT_OPTUSE;
            break;
        default:
            chars = Extends::OptionUsage(optarg, longopt);
        }
        return chars;
    }
    virtual const char_t* Options(const struct option*& longopts) {
        int err = 0;
        static const char_t* chars = XOS_APP_CONSOLE_MT_MAIN_OPTIONS_CHARS;
        static struct option optstruct[]= {
            XOS_APP_CONSOLE_MT_MAIN_OPTIONS_OPTIONS
            {0, 0, 0, 0}};
        longopts = optstruct;
        return chars;
    }
    virtual const char_t* Arguments(const char_t**& argv) {
        static const char_t* _args = XOS_APP_CONSOLE_MT_MAIN_ARGS;
        static const char_t* _argv[] = {
            XOS_APP_CONSOLE_MT_MAIN_ARGV
            0};
        argv = _argv;
        return _args;
    }
}; /// class _EXPORTED_ MainOptT
typedef MainOptT<> MainOpt;

} /// namespace mt
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_MT_MAINOPT_HXX_
