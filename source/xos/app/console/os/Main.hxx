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
#ifndef _XOS_APP_CONSOLE_OS_MAIN_HXX_
#define _XOS_APP_CONSOLE_OS_MAIN_HXX_

#include "xos/app/console/os/MainOpt.hxx"

namespace xos {
namespace app {
namespace console {
namespace os {

///////////////////////////////////////////////////////////////////////
///  Class: MainT
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = MainOpt::Implements, class TExtends = MainOpt>

class _EXPORTED_ MainT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef MainT Derives;

    typedef typename Implements::string_t string_t;
    typedef typename Implements::chars_t chars_t;
    typedef typename Implements::char_t char_t;

    MainT(): _run(0), _windowsRun(0) {
    }
    virtual ~MainT() {
    }
private:
    MainT(const MainT& copy): _run(0), _windowsRun(0) {
    }

protected:
    int (Derives::*_run)(int argc, char_t**argv, char_t** env);
    virtual int Run(int argc, char_t**argv, char_t** env) {
        int err = 0;
        if ((_run)) {
            err = (this->*_run)(argc, argv, env);
        } else {
            err = OsRun(argc, argv, env);
        }
        return err;
    }
    virtual int OsRun(int argc, char_t**argv, char_t** env) {
        int err = 0;
        return err;
    }
    virtual int SolarisRun(int argc, char_t**argv, char_t** env) {
        int err = 0;
        return err;
    }
    virtual int PosixRun(int argc, char_t**argv, char_t** env) {
        int err = 0;
        return err;
    }
    virtual int LinuxRun(int argc, char_t**argv, char_t** env) {
        int err = 0;
        return err;
    }
    virtual int OsxRun(int argc, char_t**argv, char_t** env) {
        int err = 0;
        return err;
    }
    int (Derives::*_windowsRun)(int argc, char_t**argv, char_t** env);
    virtual int WindowsRun(int argc, char_t**argv, char_t** env) {
        int err = 0;
        if ((_windowsRun)) {
            err = (this->*_windowsRun)(argc, argv, env);
        } else {
            err = WindowsCrtRun(argc, argv, env);
        }
        return err;
    }
    virtual int WindowsCrtRun(int argc, char_t**argv, char_t** env) {
        int err = 0;
        return err;
    }
    virtual int WindowsNativeRun(int argc, char_t**argv, char_t** env) {
        int err = 0;
        return err;
    }
    virtual int OnWindowsOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        _run = &Derives::WindowsRun;
        return err;
    }
    virtual int OnOsxOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        _run = &Derives::OsxRun;
        return err;
    }
    virtual int OnLinuxOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        _run = &Derives::LinuxRun;
        return err;
    }
    virtual int OnPosixOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        _run = &Derives::PosixRun;
        return err;
    }
    virtual int OnSolarisOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        _run = &Derives::SolarisRun;
        return err;
    }
    virtual int OnNativeRuntimeOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        _windowsRun = &Derives::WindowsNativeRun;
        return err;
    }
    virtual int OnClibRuntimeOption
    (int optval, const char_t* optarg,
     const char_t* optname, int optind,
     int argc, char_t**argv, char_t**env) {
        int err = 0;
        _windowsRun = &Derives::WindowsCrtRun;
        return err;
    }
}; /// class _EXPORTED_ MainT
typedef MainT<> Main;

} /// namespace os
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_OS_MAIN_HXX_
