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
///   File: Logged.hxx
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_BASE_LOGGED_HXX_
#define _XOS_BASE_LOGGED_HXX_

#include "xos/base/Base.hxx"

#define XOS_TIMED_LOGGED_THREASHOLD_MILLISECONDS 500

namespace xos {

typedef ImplementBase LoggedTImplements;
///////////////////////////////////////////////////////////////////////
///  Class: LoggedT
///////////////////////////////////////////////////////////////////////
template <class TImplements = LoggedTImplements>
class _EXPORTED_ LoggedT: virtual public TImplements {
public:
    typedef TImplements Implements;

    virtual bool SetIsLogged(bool to = true) {
        return IsLogged();
    }
    virtual bool IsLogged() const {
        return true;
    }
    virtual bool SetIsErrLogged(bool to = true) {
        return IsErrLogged();
    }
    virtual bool IsErrLogged() const {
        return true;
    }
    virtual mseconds_t SetTimedLoggedThreasholdMilliseconds(mseconds_t to) {
        return TimedLoggedThreasholdMilliseconds();
    }
    virtual mseconds_t TimedLoggedThreasholdMilliseconds() const {
        return XOS_TIMED_LOGGED_THREASHOLD_MILLISECONDS;
    }
};
typedef LoggedT<> Logged;

namespace extended {
typedef xos::Logged LoggedTImplements;
typedef Base LoggedTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: LoggedT
///////////////////////////////////////////////////////////////////////
template <class TImplements = LoggedTImplements, class TExtends = LoggedTExtends>
class _EXPORTED_ LoggedT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    LoggedT(bool isLogged, bool isErrLogged)
    : _isLogged(isLogged), _isErrLogged(isErrLogged) {
    }
    LoggedT(bool isLogged): _isLogged(isLogged), _isErrLogged(isLogged) {
    }
    LoggedT(const LoggedT &copy)
    : _isLogged(copy.IsLogged()), _isErrLogged(copy.IsErrLogged()) {
    }
    LoggedT(): _isLogged(true), _isErrLogged(true) {
    }
    virtual ~LoggedT() {
    }
    
    virtual bool SetIsLogged(bool to = true) {
        _isLogged = to;
        return _isLogged;
    }
    virtual bool IsLogged() const {
        return _isLogged;
    }
    virtual bool SetIsErrLogged(bool to = true) {
        _isErrLogged = to;
        return _isErrLogged;
    }
    virtual bool IsErrLogged() const {
        return _isErrLogged;
    }

protected:
    bool _isLogged, _isErrLogged;
};
typedef LoggedT<> Logged;
} /// namespace extended

} /// namespace xos

#endif /// _XOS_BASE_LOGGED_HXX_
