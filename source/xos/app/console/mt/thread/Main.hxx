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
///   Date: 6/2/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_APP_CONSOLE_MT_THREAD_MAIN_HXX_
#define _XOS_APP_CONSOLE_MT_THREAD_MAIN_HXX_

#include "xos/mt/os/Thread.hxx"
#include "xos/mt/posix/Thread.hxx"
#include "xos/mt/apple/osx/Thread.hxx"
#include "xos/mt/microsoft/windows/crt/Thread.hxx"
#include "xos/app/console/mt/thread/MainOpt.hxx"

namespace xos {
namespace app {
namespace console {
namespace mt {
namespace thread {

typedef MainOpt::Implements MainTImplements;
typedef MainOpt MainTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: MainT
///////////////////////////////////////////////////////////////////////
template <class TImplements = MainTImplements, class TExtends = MainTExtends>
class _EXPORT_CLASS MainT: virtual public ::xos::Ran, virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef MainT Derives;

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
    virtual void Run() {
        this->OutLLn(__LOCATION__, "...", NULL);
    }
    template <class TThread>
    int RunT(int argc, char_t**argv, char_t** env) {
        int err = 0;
        this->OutLLn(__LOCATION__, "try {...", NULL);
        try {
            TThread thread(*this);
            Join join(thread);
            this->OutLLn(__LOCATION__, "...} try", NULL);
        } catch (...) {
            this->OutLLn(__LOCATION__, "...catch (...)", NULL);
        }
        return err;
    }

    virtual int OsRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::os::Thread >(argc, argv, env);
        return err;
    }
    virtual int PosixRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::posix::Thread >(argc, argv, env);
        return err;
    }
    virtual int OsxRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::apple::osx::Thread >(argc, argv, env);
        return err;
    }
    virtual int WindowsRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::microsoft::windows::crt::Thread >(argc, argv, env);
        return err;
    }
}; /// class _EXPORT_CLASS MainT
typedef MainT<> Main;

} /// namespace thread
} /// namespace mt
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_MT_THREAD_MAIN_HXX_
