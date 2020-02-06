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
///   Date: 5/6/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_CONSOLE_MAIN_HXX_
#define _XOS_CONSOLE_MAIN_HXX_

#include "xos/console/MainArg.hxx"

namespace xos {
namespace console {

///////////////////////////////////////////////////////////////////////
///  Class: MainT
///////////////////////////////////////////////////////////////////////
template 
<class TMainImplement = MainImplement,
 class TImplements = TMainImplement, class TExtends = Base>

class _EXPORTED_ MainT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef TMainImplement main_t;
    typedef typename Implements::io_t io_t;
    typedef typename Implements::char_t char_t;
    typedef typename Implements::endchar_t endchar_t;
    static const typename Implements::endchar_t endchar = Implements::endchar;

    MainT(Locked& locked)
    : _locked(locked), _didArguments(false), _didUsage(false), _didRun(false) {
        main_t*& theMain = main_t::TheMain();
        if (!(theMain)) {
            theMain = this;
        }
    }
    MainT()
    : _locked(io_t::TheIo(*this)), 
      _didArguments(false), _didUsage(false), _didRun(false) {
        main_t*& theMain = main_t::TheMain();
        if (!(theMain)) {
            theMain = this;
        }
    }
    virtual ~MainT() {
        main_t*& theMain = main_t::TheMain();
        if ((this == theMain)) {
            theMain = 0;
        }
    }
private:
    MainT(const MainT &copy)
    : _didArguments(false), _didUsage(false), _didRun(false) {
    }

protected:
    virtual int Run(int argc, char_t**argv, char_t** env) {
        int err = 0;
        return err;
    }
    virtual int BeforeRun(int argc, char_t**argv, char_t** env) {
        int err = 0;
        return err;
    }
    virtual int AfterRun(int argc, char_t**argv, char_t** env) {
        int err = 0;
        return err;
    }

    virtual int operator()(int argc, char_t**argv, char_t** env) {
        int err = 0;
        if (!(err = BeforeRun(argc, argv, env))) {
            int err2 = 0;
            if (!(DidRun())) {
                err = Run(argc, argv, env);
            }
            if ((err2 = AfterRun(argc, argv, env))) {
                if (!err) err = err2;
            }
        }
        return err;
    }
    virtual int Before(int argc, char_t**argv, char_t** env) {
        int err = 0;
        if (!(err = this->BeforeGetArguments(argc, argv, env))) {
            int err2 = 0;
            if (!DidArguments()) {
                err = this->GetArguments(argc, argv, env);
            }
            if ((err2 = this->AfterGetArguments(argc, argv, env))) {
                if (!err) err = err2;
            }
        }
        return err;
    }
    virtual int After(int argc, char_t**argv, char_t** env) {
        int err = 0;
        return err;
    }

    virtual bool SetDidArguments(bool to = true) {
        _didArguments = to;
        return _didArguments;
    }
    virtual bool DidArguments() const {
        return _didArguments;
    }
    virtual bool SetDidUsage(bool to = true) {
        _didRun = to;
        return _didUsage = to;
    }
    virtual bool DidUsage() const {
        return _didUsage;
    }
    virtual bool SetDidRun(bool to = true) {
        return _didRun = to;
    }
    virtual bool DidRun() const {
        return _didRun;
    }

    virtual bool Lock() { 
        if (&_locked != this) {
            return _locked.Lock();
        }
        return true; 
    }
    virtual LockStatus TryLock() { 
        if (&_locked != this) {
            return _locked.TryLock();
        }
        return LockSuccess; 
    }
    virtual LockStatus TimedLock(mseconds_t milliseconds) { 
        if (&_locked != this) {
            return _locked.TimedLock(milliseconds);
        }
        return LockSuccess; 
    }
    virtual LockStatus UntimedLock() { 
        if (&_locked != this) {
            return _locked.UntimedLock();
        }
        return LockSuccess; 
    }
    virtual bool Unlock() { 
        if (&_locked != this) {
            return _locked.Unlock();
        }
        return true; 
    }

protected:
    Locked& _locked;
    bool _didArguments, _didUsage, _didRun;
};
typedef MainT<> Main;

} /// namespace console
} /// namespace xos

#endif /// _XOS_CONSOLE_MAIN_HXX_
