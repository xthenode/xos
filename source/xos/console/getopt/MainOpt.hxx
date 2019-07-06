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
///   Date: 5/6/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_CONSOLE_GETOPT_MAINOPT_HXX_
#define _XOS_CONSOLE_GETOPT_MAINOPT_HXX_

#include "xos/console/MainArg.hxx"
#include <getopt.h>

namespace xos {
typedef int MAIN_OPT_ARGUMENT_T;
enum {
    MAIN_OPT_ARGUMENT_NONE     = 0,
    MAIN_OPT_ARGUMENT_REQUIRED = 1,
    MAIN_OPT_ARGUMENT_OPTIONAL = 2
};
} /// namespace xos

#define XOS_CONSOLE_MAIN_HELP_OPT "help"
#define XOS_CONSOLE_MAIN_HELP_OPTARG_REQUIRED MAIN_OPT_ARGUMENT_NONE
#define XOS_CONSOLE_MAIN_HELP_OPTARG_RESULT 0
#define XOS_CONSOLE_MAIN_HELP_OPTARG ""
#define XOS_CONSOLE_MAIN_HELP_OPTUSE "Usage options"
#define XOS_CONSOLE_MAIN_HELP_OPTVAL_S "h"
#define XOS_CONSOLE_MAIN_HELP_OPTVAL_C 'h'
#define XOS_CONSOLE_MAIN_HELP_OPTION \
   {XOS_CONSOLE_MAIN_HELP_OPT, \
    XOS_CONSOLE_MAIN_HELP_OPTARG_REQUIRED, \
    XOS_CONSOLE_MAIN_HELP_OPTARG_RESULT, \
    XOS_CONSOLE_MAIN_HELP_OPTVAL_C}, \

#define XOS_CONSOLE_MAIN_OPTIONS_CHARS \
    XOS_CONSOLE_MAIN_HELP_OPTVAL_S

#define XOS_CONSOLE_MAIN_OPTIONS_OPTIONS \
    XOS_CONSOLE_MAIN_HELP_OPTION

namespace xos {
namespace console {
namespace getopt {

///////////////////////////////////////////////////////////////////////
///  Class: MainOptT
///////////////////////////////////////////////////////////////////////
template 
<typename TOption = struct option, class TImplements = console::MainImplement>

class _EXPORT_CLASS MainOptT: virtual public TImplements {
public:
    typedef TImplements Implements;

    typedef TOption option_t;
    typedef typename Implements::string_t string_t;
    typedef typename Implements::chars_t chars_t;
    typedef typename Implements::char_t char_t;
    typedef typename Implements::endchar_t endchar_t;
    static const typename Implements::endchar_t endchar = Implements::endchar;

protected:
    virtual int BeforeGetArguments(int argc, char_t**argv, char_t** env) {
        int err = 0;
        if (!(err = BeforeGetOptions(argc, argv, env))) {
            int err2 = 0;
            if (!DidOptions()) {
                err = GetOptions(argc, argv, env);
            }
            if ((err2 = AfterGetOptions(argc, argv, env))) {
                if (!err) err = err2;
            }
        }
        return err;
    }

    virtual int GetOptions(int argc, char_t**argv, char_t** env) {
        int err = 1;
        return err;
    }
    virtual int BeforeGetOptions(int argc, char_t**argv, char_t** env) {
        int err = 0;
        return err;
    }
    virtual int AfterGetOptions(int argc, char_t**argv, char_t** env) {
        int err = 0;
        return err;
    }

    virtual int OnUsageOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t** argv, char_t** env) {
        int err = this->Usage(argc, argv, env);
        return err;
    }
    virtual int OnInvalidOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t** argv, char_t** env) {
        int err = 1;
        return err;
    }
    virtual int OnInvalidOptionArg
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t** argv, char_t** env) {
        int err = 1;
        return err;
    }

    virtual int OnOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t** argv, char_t** env) {
        int err = 0;
        switch(optval) {
        default:
            err = OnInvalidOption
            (optval, optarg, optname, optind, argc, argv, env);
            break;
        }
        return err;
    }

    virtual const char_t* Options(const option_t*& longopts) {
        static const char_t* chars = 0;
        longopts = 0;
        return chars;
    }

    virtual bool SetDidOptions(bool to = true) {
        return DidOptions();
    }
    virtual bool DidOptions() const {
        return false;
    }
};
typedef MainOptT<> MainOptImplements;

///////////////////////////////////////////////////////////////////////
///  Class: MainOpt
///////////////////////////////////////////////////////////////////////
class _EXPORT_CLASS MainOpt: virtual public MainOptImplements {
public:
    typedef MainOptImplements Implements;

    typedef Implements::option_t option_t;
    typedef Implements::string_t string_t;
    typedef Implements::chars_t chars_t;
    typedef Implements::char_t char_t;
    typedef Implements::endchar_t endchar_t;
    static const Implements::endchar_t endchar = Implements::endchar;

protected:
    virtual int Usage(int argc, char_t** argv, char_t** env) {
        int err = 0;
        const char_t* arg = 0;
        const char_t** args = 0;
        const char_t* argstring = this->Arguments(args);
        const option_t* longopts = 0;
        const char_t* optstring = this->Options(longopts);
        const char_t* name = this->UsageName(argc, argv, env);

        this->OutLLn
        ("Usage: ", (name)?(name):(""),
         (optstring)?(" [options]"):(""),
         (argstring)?(" "):(""), (argstring)?(argstring):(""), NULL);

        if ((optstring) && (longopts)) {
            char_t optVal[2] = {0, 0};
            
            this->OutLn();
            this->OutLn("Options:");

            for (;(longopts->name); ++longopts) {
                const char_t* optarg =
                (MAIN_OPT_ARGUMENT_REQUIRED == longopts->has_arg)?(" arg"):
                ((MAIN_OPT_ARGUMENT_OPTIONAL == longopts->has_arg)?(" [arg]"):(""));
                const char_t* optusage = this->OptionUsage(optarg, longopts);
                const char_t* optargSeparator = (optarg[0])?(" "):("");
                const char_t* optusageSeparator = "  ";

                optVal[0] = longopts->val;

                this->OutLLn
                (" -", optVal, " --", longopts->name,
                 optargSeparator, optarg, optusageSeparator, optusage, NULL);
            }
        }
        if ((argstring) && (args)) {
            this->OutLn();
            this->OutLn("Arguments:");

            for (;(arg = (*args)); ++args) {
                this->OutLLn(" ", arg, NULL);
            }
        }
        this->SetDidUsage();
        return err;
    }

    virtual int GetOptions(int argc, char_t**argv, char_t** env) {
        int err = 0;
        int longindex = 0;
        const option_t* longopts = 0;
        char optvaluename[2] = {0,0};
        const char_t* optname = optvaluename;
        const char_t* optstring = 0;
        int optvalue = 0;

        if ((optstring = this->Options(longopts)) && (longopts)) {
            opterr = 0;
            while (0 <= (optvalue = getopt_long(argc, argv, optstring, longopts, &longindex))) {
                optvaluename[0] = optvalue;
                optname = (longindex)?(longopts[longindex].name):(optvaluename);

                if ((err = OnOption(optvalue, optarg, optname, longindex, argc, argv, env))) {
                    break;
                }
            }
        }
        return err;
    }

    virtual int OnInvalidOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t** argv, char_t** env) {
        int err = 1;
        int argind = optind+1;
        const char_t* arg = (argind<argc)?(argv[argind]):("?");
        this->ErrLLn("invalid option \"", arg, "\"", NULL);
        return err;
    }
    virtual int OnInvalidOptionArg
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t** argv, char_t** env) {
        int err = 1;
        return err;
    }
    virtual int OnOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t** argv, char_t** env) {
        int err = 0;
        switch(optval) {
        case XOS_CONSOLE_MAIN_HELP_OPTVAL_C:
            err = OnUsageOption
            (optval, optarg, optname, optind, argc, argv, env);
            break;
        default:
            err = OnInvalidOption
            (optval, optarg, optname, optind, argc, argv, env);
            break;
        }
        return err;
    }
    virtual const char_t* OptionUsage
    (const char_t*& optarg, const option_t* longopt) {
        const char_t* chars = "";
        switch(longopt->val) {
        case XOS_CONSOLE_MAIN_HELP_OPTVAL_C:
            chars = XOS_CONSOLE_MAIN_HELP_OPTUSE;
            break;
        default:
            break;
        }
        return chars;
    }
    virtual const char_t* Options(const option_t*& longopts) {
        static const char_t* chars = XOS_CONSOLE_MAIN_OPTIONS_CHARS;
        static option_t optstruct[] = {
            XOS_CONSOLE_MAIN_OPTIONS_OPTIONS
            {0, 0, 0, 0}};
        longopts = optstruct;
        return chars;
    }
};

} /// namespace getopt
} /// namespace console
} /// namespace xos

#endif /// _XOS_CONSOLE_GETOPT_MAINOPT_HXX_
