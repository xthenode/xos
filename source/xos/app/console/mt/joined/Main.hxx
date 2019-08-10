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
///   Date: 7/17/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_APP_CONSOLE_MT_JOINED_MAIN_HXX_
#define _XOS_APP_CONSOLE_MT_JOINED_MAIN_HXX_

#include "xos/mt/Joined.hxx"
#include "xos/app/console/mt/joined/MainOpt.hxx"

namespace xos {
namespace app {
namespace console {
namespace mt {
namespace joined {

///////////////////////////////////////////////////////////////////////
///  Class: MainT
///////////////////////////////////////////////////////////////////////
template 
<class TRan = ::xos::Ran, class TImplements = MainOpt::Implements, class TExtends = MainOpt>

class _EXPORT_CLASS MainT: virtual public TRan, virtual public TImplements, public TExtends {
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
    virtual void Run() {
        mseconds_t timeout = 0;
        bool infinite = this->InfiniteSleep(timeout);
        if ((infinite) || (!timeout)) {
            this->OutLLn(__LOCATION__, "...", NULL);
        } else {
            this->OutLLn(__LOCATION__, "sleep(timeout = ", UnsignedToString(timeout).Chars(), ")...", NULL);
            ::xos::mt::os::Sleep sleep(timeout);
            this->OutLLn(__LOCATION__, "...sleep(timeout = ", UnsignedToString(timeout).Chars(), ")", NULL);
        }
    }
    virtual void Run(::xos::Joined& joined) {
        mseconds_t timeout = 0;
        bool infinite = this->InfiniteTimeout(timeout);
        this->OutLLn(__LOCATION__, "try {...", NULL);
        try {
            if ((infinite)) {
                this->OutLLn(__LOCATION__, "::xos::Join join(joined)...", NULL);
                ::xos::Join join(joined);
                this->OutLLn(__LOCATION__, "...::xos::Join join(joined)", NULL);
            } else {
                this->OutLLn(__LOCATION__, "::xos::Join join(joined, timeout = ", UnsignedToString(timeout).Chars(), ")...", NULL);
                ::xos::Join join(joined, timeout);
                this->OutLLn(__LOCATION__, "...::xos::Join join(joined, timeout = ", UnsignedToString(timeout).Chars(), ")", NULL);
            }
            this->OutLLn(__LOCATION__, "...} try", NULL);
        } catch(const Exception& e) {
            this->OutLLn(__LOCATION__, "...catch (const Exception& e.Status = \"", e.StatusToString().Chars(), "\")", NULL);
        } catch(...) {
            this->OutLLn(__LOCATION__, "...catch (...)", NULL);
        }
    }
    template <class TJoined>
    int RunT(int argc, char_t**argv, char_t** env) {
        int err = 0;
        this->OutLLn(__LOCATION__, "try {...", NULL);
        try {
            ::xos::mt::JoinedT<TJoined> joined(*this);
            Run(joined);
            this->OutLLn(__LOCATION__, "...} try", NULL);
        } catch(const Exception& e) {
            this->OutLLn(__LOCATION__, "...catch (const Exception& e.Status = \"", e.StatusToString().Chars(), "\")", NULL);
        } catch (...) {
            this->OutLLn(__LOCATION__, "...catch (...)", NULL);
        }
        return err;
    }

    virtual int WindowsCrtRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::microsoft::windows::crt::Thread >(argc, argv, env);
        return err;
    }
    virtual int WindowsNativeRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::microsoft::windows::Thread >(argc, argv, env);
        return err;
    }
    virtual int OsxRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::apple::osx::Thread >(argc, argv, env);
        return err;
    }
    virtual int LinuxRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::linux::Thread >(argc, argv, env);
        return err;
    }
    virtual int PosixRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::posix::Thread >(argc, argv, env);
        return err;
    }
    virtual int OsRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::os::Thread >(argc, argv, env);
        return err;
    }
    virtual int DerivedRun(int argc, char_t**argv, char_t** env) {
        int err = RunT< ::xos::mt::derived::Thread >(argc, argv, env);
        return err;
    }
}; /// class _EXPORT_CLASS MainT
typedef MainT<> Main;

} /// namespace joined
} /// namespace mt
} /// namespace console
} /// namespace app
} /// namespace xos

#endif /// _XOS_APP_CONSOLE_MT_JOINED_MAIN_HXX_
