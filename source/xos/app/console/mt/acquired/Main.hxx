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
#ifndef _XOS_APP_CONSOLE_MT_ACQUIRED_MAIN_HXX_
#define _XOS_APP_CONSOLE_MT_ACQUIRED_MAIN_HXX_

#include "xos/mt/Acquired.hxx"
#include "xos/app/console/mt/acquired/MainOpt.hxx"

namespace xos {
namespace app {
namespace console {
namespace mt {
namespace acquired {

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
    virtual int Run(::xos::Acquired& acquired) {
        int err = 1;
        mseconds_t timeout = 0;
        bool infinite = this->InfiniteTimeout(timeout);

        for (unsigned tries = 1, tried = 0; tried < tries; ++tried) {
            this->OutLLn(__LOCATION__, "try {...", NULL);
            try {
                if ((infinite)) {
                    this->OutLLn(__LOCATION__, "Acquire acquire(acquired)...", NULL);
                    Acquire acquire(acquired);
                    this->OutLLn(__LOCATION__, "...Acquire acquire(acquired)", NULL);
                } else {
                    this->OutLLn(__LOCATION__, "Acquire acquire(acquired, timeout = ", UnsignedToString(timeout).Chars(), ")...", NULL);
                    Acquire acquire(acquired, timeout);
                    this->OutLLn(__LOCATION__, "...Acquire acquire(acquired, timeout = ", UnsignedToString(timeout).Chars(), ")", NULL);
                }
                err = 0;
                this->OutLLn(__LOCATION__, "...} try", NULL);
            } catch(const AcquireException& e) {
                this->OutLLn(__LOCATION__, "...catch (const AcquireException& e.Status = \"", e.StatusToString().Chars(), "\")", NULL);
                if (AcquireBusy == (e.Status())) {
                    acquired.Release();
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
    template <class TAcquired>
    int RunT(int argc, char_t**argv, char_t** env) {
        int err = 0;
        mseconds_t timeout = 0;
        bool infinite = this->InfiniteTimeout(timeout);

        this->OutLLn(__LOCATION__, "try {...", NULL);
        try {
            ::xos::mt::AcquiredT< TAcquired > acquired;
            if ((infinite)) {
                ::xos::Release release(acquired);
                Run(acquired);
            } else {
                Run(acquired);
            }
            this->OutLLn(__LOCATION__, "...} try", NULL);
        } catch (...) {
            this->OutLLn(__LOCATION__, "...catch (...)", NULL);
        }
        return err;
    }

    virtual int WindowsRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::microsoft::windows::Semaphore >(argc, argv, env);
        return err;
    }
    virtual int OsxRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::apple::mach::Semaphore >(argc, argv, env);
        return err;
    }
    virtual int PosixRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::posix::Semaphore >(argc, argv, env);
        return err;
    }
    virtual int OsRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::os::Semaphore >(argc, argv, env);
        return err;
    }
}; /// class _EXPORTED_ MainT
typedef MainT<> Main;

} /// namespace acquired
} /// namespace mt
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_MT_ACQUIRED_MAIN_HXX_
