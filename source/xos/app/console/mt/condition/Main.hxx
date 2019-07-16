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
///   Date: 7/4/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_APP_CONSOLE_MT_CONDITION_MAIN_HXX_
#define _XOS_APP_CONSOLE_MT_CONDITION_MAIN_HXX_

#include "xos/mt/os/Condition.hxx"
#include "xos/mt/posix/Condition.hxx"
#include "xos/mt/apple/osx/Condition.hxx"
#include "xos/mt/microsoft/windows/Condition.hxx"
#include "xos/app/console/mt/condition/MainOpt.hxx"

namespace xos {
namespace app {
namespace console {
namespace mt {
namespace condition {

///////////////////////////////////////////////////////////////////////
///  Class: MainT
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = MainOpt::Implements, class TExtends = MainOpt>

class _EXPORT_CLASS MainT: virtual public TImplements, public TExtends {
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
    virtual int Run(::xos::Signaled& signaled) {
        int err = 1;
        mseconds_t timeout = 0;
        bool infinite = this->InfiniteTimeout(timeout);
        for (unsigned tries = 1, tried = 0; tried < tries; ++tried) {
            this->OutLLn(__LOCATION__, "try {...", NULL);
            try {
                if ((infinite)) {
                    this->OutLLn(__LOCATION__, "signaled.Signal()", NULL);
                    signaled.Signal();
                    this->OutLLn(__LOCATION__, "Wait wait(signaled)...", NULL);
                    Wait wait(signaled, timeout);
                    this->OutLLn(__LOCATION__, "...Wait wait(signaled)", NULL);
                } else {
                    this->OutLLn(__LOCATION__, "Wait wait(signaled, timeout = ", UnsignedToString(timeout).Chars(), " ms)...", NULL);
                    Wait wait(signaled, timeout);
                    this->OutLLn(__LOCATION__, "...Wait wait(signaled, timeout = ", UnsignedToString(timeout).Chars(), " ms)", NULL);
                }
                err = 0;
                this->OutLLn(__LOCATION__, "...} try", NULL);
            } catch(const WaitException& e) {
                this->OutLLn(__LOCATION__, "...catch (const WaitException& e.Status = \"", e.StatusToString().Chars(), "\")", NULL);
                if (WaitBusy == (e.Status())) {
                    this->OutLLn(__LOCATION__, "signaled.Signal()", NULL);
                    signaled.Signal();
                    tries = 2;
                }
            } catch(const Exception& e) {
                this->OutLLn(__LOCATION__, "...catch (const Exception& e.Status = \"", e.StatusToString().Chars(), "\")", NULL);
            } catch(...) {
                this->OutLLn(__LOCATION__, "...catch (...)", NULL);
            }
        }
        return err;
    }
    template <class TCondition, class TMutex>
    int RunT(int argc, char_t**argv, char_t** env) {
        int err = 1;
        this->OutLLn(__LOCATION__, "try {...", NULL);
        try {
            bool signaled = false;
            TMutex mutex(true);
            TCondition condition(mutex, signaled);
            err = Run(condition);
            this->OutLLn(__LOCATION__, "...} try", NULL);
        } catch(const Exception& e) {
            this->OutLLn(__LOCATION__, "...catch (const Exception& e.Status = \"", e.StatusToString().Chars(), "\")", NULL);
        } catch(...) {
            this->OutLLn(__LOCATION__, "...catch (...)", NULL);
        }
        return err;
    }
    virtual int WindowsRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::microsoft::windows::Condition, ::xos::mt::microsoft::windows::Mutex >(argc, argv, env);
        return err;
    }
    virtual int OsxRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::apple::osx::Condition, ::xos::mt::apple::osx::Mutex >(argc, argv, env);
        return err;
    }
    virtual int PosixRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::posix::Condition, ::xos::mt::posix::Mutex >(argc, argv, env);
        return err;
    }
    virtual int OsRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::os::Condition, ::xos::mt::os::Mutex >(argc, argv, env);
        return err;
    }
}; /// class _EXPORT_CLASS MainT
typedef MainT<> Main;

} /// namespace condition
} /// namespace mt
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_MT_CONDITION_MAIN_HXX_
