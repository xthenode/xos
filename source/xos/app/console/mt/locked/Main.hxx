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
///   Date: 7/11/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_APP_CONSOLE_MT_LOCKED_MAIN_HXX_
#define _XOS_APP_CONSOLE_MT_LOCKED_MAIN_HXX_

#include "xos/mt/Locked.hxx"
#include "xos/app/console/mt/locked/MainOpt.hxx"

namespace xos {
namespace app {
namespace console {
namespace mt {
namespace locked {

///////////////////////////////////////////////////////////////////////
///  Class: MainT
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = MainOpt::Implements, class TExtends = MainOpt>

class _EXPORTED_ MainT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    MainT() {
    }
    virtual ~MainT() {
    }
private:
    MainT(const MainT& copy) {
    }

protected:
    virtual void Run(::xos::Locked& locked) {
        mseconds_t timeout = 0;
        bool infinite = this->InfiniteTimeout(timeout);
        this->OutLLn(__LOCATION__, "try {...", NULL);
        try {
            if ((infinite)) {
                this->OutLLn(__LOCATION__, "::xos::Lock lock(locked)...", NULL);
                ::xos::Lock lock(locked);
                this->OutLLn(__LOCATION__, "...::xos::Lock lock(locked)", NULL);
            } else {
                this->OutLLn(__LOCATION__, "::xos::Lock lock(locked, timeout = ", UnsignedToString(timeout).Chars(), ")...", NULL);
                ::xos::Lock lock(locked, timeout);
                this->OutLLn(__LOCATION__, "...::xos::Lock lock(locked, timeout = ", UnsignedToString(timeout).Chars(), ")", NULL);
            }
            this->OutLLn(__LOCATION__, "...} try", NULL);
        } catch(const Exception& e) {
            this->OutLLn(__LOCATION__, "...catch (const Exception& e.Status = \"", e.StatusToString().Chars(), "\")", NULL);
        } catch(...) {
            this->OutLLn(__LOCATION__, "...catch (...)", NULL);
        }
    }
    template <class TLocked>
    int RunT(int argc, char_t**argv, char_t** env) {
        int err = 0;
        mseconds_t timeout = 0;
        bool infinite = this->InfiniteTimeout(timeout);
        this->OutLLn(__LOCATION__, "try {...", NULL);
        try {
            ::xos::mt::LockedT<TLocked> locked;
            if ((infinite)) {
                Run(locked);
            } else {
                try {
                    ::xos::Lock lock(locked);
                    Run(locked);
                } catch (...) {
                }
            }
            this->OutLLn(__LOCATION__, "...} try", NULL);
        } catch(const Exception& e) {
            this->OutLLn(__LOCATION__, "...catch (const Exception& e.Status = \"", e.StatusToString().Chars(), "\")", NULL);
        } catch (...) {
            this->OutLLn(__LOCATION__, "...catch (...)", NULL);
        }
        return err;
    }

    virtual int WindowsRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::microsoft::windows::Mutex >(argc, argv, env);
        return err;
    }
    virtual int OsxRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::apple::osx::Mutex >(argc, argv, env);
        return err;
    }
    virtual int PosixRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::posix::Mutex >(argc, argv, env);
        return err;
    }
    virtual int OsRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::os::Mutex >(argc, argv, env);
        return err;
    }
}; /// class _EXPORTED_ MainT
typedef MainT<> Main;

} /// namespace locked
} /// namespace mt
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_MT_LOCKED_MAIN_HXX_
