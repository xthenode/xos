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
///   File: Interface.hxx
///
/// Author: $author$
///   Date: 5/6/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_LOGGER_INTERFACE_HXX_
#define _XOS_LOGGER_INTERFACE_HXX_

#include "xos/logger/Output.hxx"

namespace xos {
namespace logger {

typedef Output InterfaceTImplements;
///////////////////////////////////////////////////////////////////////
///  Class: InterfaceT
///////////////////////////////////////////////////////////////////////
template <class TImplements = InterfaceTImplements>
class _EXPORT_CLASS InterfaceT: virtual public TImplements {
public:
    typedef TImplements Implements;

    typedef Location::char_t char_t;

    virtual bool Init() {
        return true;
    }
    virtual bool Fini() {
        return true;
    }

    virtual void Log
    (const Level& level, const Location& location, const Message& message) {
        ::xos::Lock lock(*this);
        if ((this->IsEnabledFor(level))) {
            this->Out(location);
            this->Out(message);
            this->OutLn();
        }
    }
    virtual void Log
    (const Level& level, const Function& function, const Message& message) {
        ::xos::Lock lock(*this);
        if ((this->IsEnabledFor(level))) {
            this->Out(function);
            this->Out(message);
            this->OutLn();
        }
    }
    virtual void Log
    (const Level& level, const Message& message) {
        ::xos::Lock lock(*this);
        if ((this->IsEnabledFor(level))) {
            this->Out(message);
            this->OutLn();
        }
    }

    virtual void Log(const Location& location, const Message& message) {
        ::xos::Lock lock(*this);
        this->Out(location);
        this->Out(message);
        this->OutLn();
    }
    virtual void Log(const Function& function, const Message& message) {
        ::xos::Lock lock(*this);
        this->Out(function);
        this->Out(message);
        this->OutLn();
    }
    virtual void Log(const Message& message) {
        ::xos::Lock lock(*this);
        this->Out(message);
        this->OutLn();
    }
    virtual void Log(const char_t* _message_, size_t length) {
        const Message message(_message_, length);
        Log(message);
    }
    virtual void Log(const char_t* _message_) {
        const Message message(_message_);
        Log(message);
    }

    static InterfaceT* SetDefault(InterfaceT* to) {
        TheDefault() = to;
        return TheDefault();
    }
    static InterfaceT* Default() {
        return TheDefault();
    }
protected:
    static InterfaceT*& TheDefault() {
        static InterfaceT* theDefault = 0;
        return theDefault;
    }
};
typedef InterfaceT<> Interface;

typedef Interface BaseTImplements;
typedef xos::Base BaseTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: BaseT
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = BaseTImplements, class TExtends = BaseTExtends>

class _EXPORT_CLASS BaseT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    BaseT(xos::Locked& locked, Level level): _locked(locked), _level(level) {
        Interface*& theDefault = Implements::TheDefault();
        if (!(theDefault)) {
            theDefault = this;
        }
    }
    BaseT(xos::Locked& locked): _locked(locked), _level(this->EnabledForDefault()) {
        Interface*& theDefault = Implements::TheDefault();
        if (!(theDefault)) {
            theDefault = this;
        }
    }
    BaseT(Level level): _locked(*this), _level(level) {
        Interface*& theDefault = Implements::TheDefault();
        if (!(theDefault)) {
            theDefault = this;
        }
    }
    BaseT(): _locked(*this), _level(this->EnabledForDefault()) {
        Interface*& theDefault = Implements::TheDefault();
        if (!(theDefault)) {
            theDefault = this;
        }
    }
    virtual ~BaseT() {
        Interface*& theDefault = Implements::TheDefault();
        if ((theDefault) && (this == theDefault)) {
            theDefault = 0;
        }
    }
private:
    BaseT(const BaseT &copy): _locked(copy.Locked()), _level(copy._level) {
        Interface*& theDefault = Implements::TheDefault();
        if (!(theDefault)) {
            theDefault = this;
        }
    }

protected:
    virtual Level EnableFor(const Level& level) {
        Level enabledFor = this->EnabledFor();
        _level = level;
        return enabledFor;
    }
    virtual Level EnabledFor() const {
        return _level;
    }
    virtual bool IsEnabledFor(const Level& level) const {
        if ((level) && (level == (level & _level))) {
            return true;
        }
        return false;
    }

    virtual bool Lock() {
        if (&_locked != this) {
            return _locked.Lock(); 
        }
        return true;
    }
    virtual bool Unlock() {
        if (&_locked != this) {
            return _locked.Unlock(); 
        }
        return true;
    }
    virtual xos::Locked& Locked() const {
        if (&_locked != this) {
            return ((xos::Locked&)_locked); 
        }
        return ((xos::Locked&)*this);
    }

protected:
    xos::Locked& _locked;
    Level _level;
};
typedef BaseT<> Base;

Interface* SetDefault(Interface* to);
Interface* GetDefault();

} /// namespace logger
} /// namespace xos

///
///
///
#define XOS_INIT_LOGGER(logger_) { \
::xos::logger::Interface* logger = logger_; \
if ((logger)) { logger->Init(); } }

#define XOS_FINI_LOGGER(logger_) { \
::xos::logger::Interface* logger = logger_; \
if ((logger)) { logger->Fini(); } }

#define XOS_SET_LOGGER_LEVEL(logger_, level_) { \
::xos::logger::Interface* logger = logger_; \
if ((logger)) { logger->EnableFor(level_); } }

#define XOS_GET_LOGGER_LEVEL(logger_) \
((logger_)?(logger_->enabled_for()):(::xos::logger::level\
(::xos::logger::LevelsNone)))

#if !defined(XOS_USE_LOGGER)
#define XOS_DEFAULT_LOGGER ::xos::logger::Interface::Default()
#endif // !defined(XOS_USE_LOGGER)
#define XOS_SET_DEFAULT_LOGGER(to) ::xos::logger::Interface::SetDefault(to)

#define XOS_LOGGER_INIT() XOS_INIT_LOGGER(XOS_DEFAULT_LOGGER)
#define XOS_LOGGER_FINI() XOS_FINI_LOGGER(XOS_DEFAULT_LOGGER)

#define XOS_LOGGER_SET_LEVEL(level)  XOS_SET_LOGGER_LEVEL(XOS_DEFAULT_LOGGER, level)
#define XOS_LOGGER_GET_LEVEL(level)  (level = XOS_GET_LOGGER_LEVEL(XOS_DEFAULT_LOGGER))

///
///
///
#define XOS_LOG_LOCATION(logger_, level_, message_) { \
::xos::logger::Interface* logger = logger_; \
if ((logger)?(logger->IsEnabledFor(level_)):(false)) {\
   ::xos::logger::Message message; \
   logger->Log(level_, XOS_LOGGER_LOCATION, message << message_); } }

#define XOS_LOG_FUNCTION(logger_, level_, message_) { \
::xos::logger::Interface* logger = logger_; \
if ((logger)?(logger->IsEnabledFor(level_)):(false)) {\
   ::xos::logger::Message message; \
   logger->Log(level_, XOS_LOGGER_FUNCTION, message << message_); } }

#define XOS_LOG_PLAIN(logger_, level_, message_) { \
::xos::logger::Interface* logger = logger_; \
if ((logger)?(logger->IsEnabledFor(level_)):(false)) {\
   ::xos::logger::Message message; \
   logger->Log(level_, message << message_); } }

///
///
///
#define XOS_IF_LOGGED_LOCATION(isLogged_, logger_, level_, message_) { \
if ((isLogged_)) { XOS_LOG_LOCATION(logger_, level_, message_); } }

#define XOS_IF_LOGGED_FUNCTION(isLogged_, logger_, level_, message_) { \
if ((isLogged_)) { XOS_LOG_FUNCTION(logger_, level_, message_); } }

#define XOS_IF_LOGGED_PLAIN(isLogged_, logger_, level_, message_) { \
if ((isLogged_)) { XOS_LOG_PLAIN(logger_, level_, message_); } }

///
///
///
#define XOS_IS_LOGGED_LOCATION(logger_, level_, message_) \
{ XOS_IF_LOGGED_LOCATION(this->IsLogged(), logger_, level_, message_); }

#define XOS_IS_LOGGED_FUNCTION(logger_, level_, message_) \
{ XOS_IF_LOGGED_FUNCTION(this->IsLogged(), logger_, level_, message_); }

#define XOS_IS_LOGGED_PLAIN(logger_, level_, message_) \
{ XOS_IF_LOGGED_PLAIN(this->IsLogged(), logger_, level_, message_); }


///
///
///
#define XOS_LOG_LOCATION_FATAL(message) XOS_LOG_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_FATAL, message)
#define XOS_LOG_LOCATION_ERROR(message) XOS_LOG_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_ERROR, message)
#define XOS_LOG_LOCATION_WARN(message) XOS_LOG_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_WARN, message)
#define XOS_LOG_LOCATION_INFO(message) XOS_LOG_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_INFO, message)
#define XOS_LOG_LOCATION_DEBUG(message) XOS_LOG_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_DEBUG, message)
#define XOS_LOG_LOCATION_TRACE(message) XOS_LOG_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_TRACE, message)

#define XOS_LOG_FUNCTION_FATAL(message) XOS_LOG_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_FATAL, message)
#define XOS_LOG_FUNCTION_ERROR(message) XOS_LOG_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_ERROR, message)
#define XOS_LOG_FUNCTION_WARN(message) XOS_LOG_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_WARN, message)
#define XOS_LOG_FUNCTION_INFO(message) XOS_LOG_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_INFO, message)
#define XOS_LOG_FUNCTION_DEBUG(message) XOS_LOG_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_DEBUG, message)
#define XOS_LOG_FUNCTION_TRACE(message) XOS_LOG_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_TRACE, message)

#define XOS_LOG_PLAIN_FATAL(message) XOS_LOG_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_FATAL, message)
#define XOS_LOG_PLAIN_ERROR(message) XOS_LOG_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_ERROR, message)
#define XOS_LOG_PLAIN_WARN(message) XOS_LOG_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_WARN, message)
#define XOS_LOG_PLAIN_INFO(message) XOS_LOG_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_INFO, message)
#define XOS_LOG_PLAIN_DEBUG(message) XOS_LOG_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_DEBUG, message)
#define XOS_LOG_PLAIN_TRACE(message) XOS_LOG_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_TRACE, message)

///
///
///
#define XOS_LOG_LOCATION_DEBUG_TRACE(cond, message) \
{ if ((cond)) { XOS_LOG_LOCATION_DEBUG(message); } \
  else { XOS_LOG_LOCATION_TRACE(message); } }

#define XOS_LOG_FUNCTION_DEBUG_TRACE(cond, message) \
{ if ((cond)) { XOS_LOG_FUNCTION_DEBUG(message); } \
  else { XOS_LOG_FUNCTION_TRACE(message); } }

#define XOS_LOG_PLAIN_DEBUG_TRACE(cond, message) \
{ if ((cond)) { XOS_LOG_PLAIN_DEBUG(message); } \
  else { XOS_LOG_PLAIN_TRACE(message); } }

///
///
///
#define XOS_IF_LOGGED_LOCATION_FATAL(isLogged, message) XOS_IF_LOGGED_LOCATION(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_FATAL, message)
#define XOS_IF_LOGGED_LOCATION_ERROR(isLogged, message) XOS_IF_LOGGED_LOCATION(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_ERROR, message)
#define XOS_IF_LOGGED_LOCATION_WARN(isLogged, message) XOS_IF_LOGGED_LOCATION(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_WARN, message)
#define XOS_IF_LOGGED_LOCATION_INFO(isLogged, message) XOS_IF_LOGGED_LOCATION(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_INFO, message)
#define XOS_IF_LOGGED_LOCATION_DEBUG(isLogged, message) XOS_IF_LOGGED_LOCATION(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_DEBUG, message)
#define XOS_IF_LOGGED_LOCATION_TRACE(isLogged, message) XOS_IF_LOGGED_LOCATION(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_TRACE, message)

#define XOS_IF_LOGGED_FUNCTION_FATAL(isLogged, message) XOS_IF_LOGGED_FUNCTION(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_FATAL, message)
#define XOS_IF_LOGGED_FUNCTION_ERROR(isLogged, message) XOS_IF_LOGGED_FUNCTION(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_ERROR, message)
#define XOS_IF_LOGGED_FUNCTION_WARN(isLogged, message) XOS_IF_LOGGED_FUNCTION(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_WARN, message)
#define XOS_IF_LOGGED_FUNCTION_INFO(isLogged, message) XOS_IF_LOGGED_FUNCTION(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_INFO, message)
#define XOS_IF_LOGGED_FUNCTION_DEBUG(isLogged, message) XOS_IF_LOGGED_FUNCTION(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_DEBUG, message)
#define XOS_IF_LOGGED_FUNCTION_TRACE(isLogged, message) XOS_IF_LOGGED_FUNCTION(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_TRACE, message)

#define XOS_IF_LOGGED_PLAIN_FATAL(isLogged, message) XOS_IF_LOGGED_PLAIN(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_FATAL, message)
#define XOS_IF_LOGGED_PLAIN_ERROR(isLogged, message) XOS_IF_LOGGED_PLAIN(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_ERROR, message)
#define XOS_IF_LOGGED_PLAIN_WARN(isLogged, message) XOS_IF_LOGGED_PLAIN(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_WARN, message)
#define XOS_IF_LOGGED_PLAIN_INFO(isLogged, message) XOS_IF_LOGGED_PLAIN(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_INFO, message)
#define XOS_IF_LOGGED_PLAIN_DEBUG(isLogged, message) XOS_IF_LOGGED_PLAIN(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_DEBUG, message)
#define XOS_IF_LOGGED_PLAIN_TRACE(isLogged, message) XOS_IF_LOGGED_PLAIN(isLogged, XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_TRACE, message)

///
///
///
#define XOS_IS_LOGGED_LOCATION_FATAL(message) XOS_IS_LOGGED_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_FATAL, message)
#define XOS_IS_LOGGED_LOCATION_ERROR(message) XOS_IS_LOGGED_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_ERROR, message)
#define XOS_IS_LOGGED_LOCATION_WARN(message) XOS_IS_LOGGED_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_WARN, message)
#define XOS_IS_LOGGED_LOCATION_INFO(message) XOS_IS_LOGGED_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_INFO, message)
#define XOS_IS_LOGGED_LOCATION_DEBUG(message) XOS_IS_LOGGED_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_DEBUG, message)
#define XOS_IS_LOGGED_LOCATION_TRACE(message) XOS_IS_LOGGED_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_TRACE, message)

#define XOS_IS_LOGGED_FUNCTION_FATAL(message) XOS_IS_LOGGED_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_FATAL, message)
#define XOS_IS_LOGGED_FUNCTION_ERROR(message) XOS_IS_LOGGED_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_ERROR, message)
#define XOS_IS_LOGGED_FUNCTION_WARN(message) XOS_IS_LOGGED_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_WARN, message)
#define XOS_IS_LOGGED_FUNCTION_INFO(message) XOS_IS_LOGGED_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_INFO, message)
#define XOS_IS_LOGGED_FUNCTION_DEBUG(message) XOS_IS_LOGGED_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_DEBUG, message)
#define XOS_IS_LOGGED_FUNCTION_TRACE(message) XOS_IS_LOGGED_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_TRACE, message)

#define XOS_IS_LOGGED_PLAIN_FATAL(message) XOS_IS_LOGGED_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_FATAL, message)
#define XOS_IS_LOGGED_PLAIN_ERROR(message) XOS_IS_LOGGED_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_ERROR, message)
#define XOS_IS_LOGGED_PLAIN_WARN(message) XOS_IS_LOGGED_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_WARN, message)
#define XOS_IS_LOGGED_PLAIN_INFO(message) XOS_IS_LOGGED_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_INFO, message)
#define XOS_IS_LOGGED_PLAIN_DEBUG(message) XOS_IS_LOGGED_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_DEBUG, message)
#define XOS_IS_LOGGED_PLAIN_TRACE(message) XOS_IS_LOGGED_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_LEVEL_TRACE, message)


///
///
///
#if defined(XOS_PLAIN_LOGGING)
#define XOS_LOG_FATAL XOS_LOG_PLAIN_FATAL
#define XOS_LOG_ERROR XOS_LOG_PLAIN_ERROR
#define XOS_LOG_WARN  XOS_LOG_PLAIN_WARN
#define XOS_LOG_INFO  XOS_LOG_PLAIN_INFO
#define XOS_LOG_DEBUG XOS_LOG_PLAIN_DEBUG
#define XOS_LOG_TRACE XOS_LOG_PLAIN_TRACE
#define XOS_LOG_DEBUG_TRACE XOS_LOG_PLAIN_DEBUG_TRACE
#else // defined(XOS_PLAIN_LOGGING)
#if defined(XOS_FUNCTION_LOGGING)
#define XOS_LOG_FATAL XOS_LOG_FUNCTION_FATAL
#define XOS_LOG_ERROR XOS_LOG_FUNCTION_ERROR
#define XOS_LOG_WARN  XOS_LOG_FUNCTION_WARN
#define XOS_LOG_INFO  XOS_LOG_FUNCTION_INFO
#define XOS_LOG_DEBUG XOS_LOG_FUNCTION_DEBUG
#define XOS_LOG_TRACE XOS_LOG_FUNCTION_TRACE
#define XOS_LOG_DEBUG_TRACE XOS_LOG_FUNCTION_DEBUG_TRACE
#else // defined(XOS_FUNCTION_LOGGING)
#define XOS_LOG_FATAL XOS_LOG_LOCATION_FATAL
#define XOS_LOG_ERROR XOS_LOG_LOCATION_ERROR
#define XOS_LOG_WARN  XOS_LOG_LOCATION_WARN
#define XOS_LOG_INFO  XOS_LOG_LOCATION_INFO
#define XOS_LOG_DEBUG XOS_LOG_LOCATION_DEBUG
#define XOS_LOG_TRACE XOS_LOG_LOCATION_TRACE
#define XOS_LOG_DEBUG_TRACE XOS_LOG_LOCATION_DEBUG_TRACE
#endif // defined(XOS_FUNCTION_LOGGING)
#endif // defined(XOS_PLAIN_LOGGING)

#if defined(XOS_PLAIN_LOGGING)
#define XOS_IF_LOGGED_FATAL XOS_IF_LOGGED_PLAIN_FATAL
#define XOS_IF_LOGGED_ERROR XOS_IF_LOGGED_PLAIN_ERROR
#define XOS_IF_LOGGED_WARN  XOS_IF_LOGGED_PLAIN_WARN
#define XOS_IF_LOGGED_INFO  XOS_IF_LOGGED_PLAIN_INFO
#define XOS_IF_LOGGED_DEBUG XOS_IF_LOGGED_PLAIN_DEBUG
#define XOS_IF_LOGGED_TRACE XOS_IF_LOGGED_PLAIN_TRACE
#else // defined(XOS_PLAIN_LOGGING)
#if defined(XOS_FUNCTION_LOGGING)
#define XOS_IF_LOGGED_FATAL XOS_IF_LOGGED_FUNCTION_FATAL
#define XOS_IF_LOGGED_ERROR XOS_IF_LOGGED_FUNCTION_ERROR
#define XOS_IF_LOGGED_WARN  XOS_IF_LOGGED_FUNCTION_WARN
#define XOS_IF_LOGGED_INFO  XOS_IF_LOGGED_FUNCTION_INFO
#define XOS_IF_LOGGED_DEBUG XOS_IF_LOGGED_FUNCTION_DEBUG
#define XOS_IF_LOGGED_TRACE XOS_IF_LOGGED_FUNCTION_TRACE
#else // defined(XOS_FUNCTION_LOGGING)
#define XOS_IF_LOGGED_FATAL XOS_IF_LOGGED_LOCATION_FATAL
#define XOS_IF_LOGGED_ERROR XOS_IF_LOGGED_LOCATION_ERROR
#define XOS_IF_LOGGED_WARN  XOS_IF_LOGGED_LOCATION_WARN
#define XOS_IF_LOGGED_INFO  XOS_IF_LOGGED_LOCATION_INFO
#define XOS_IF_LOGGED_DEBUG XOS_IF_LOGGED_LOCATION_DEBUG
#define XOS_IF_LOGGED_TRACE XOS_IF_LOGGED_LOCATION_TRACE
#endif // defined(XOS_FUNCTION_LOGGING)
#endif // defined(XOS_PLAIN_LOGGING)

#if defined(XOS_PLAIN_LOGGING)
#define XOS_IS_LOGGED_FATAL XOS_IS_LOGGED_PLAIN_FATAL
#define XOS_IS_LOGGED_ERROR XOS_IS_LOGGED_PLAIN_ERROR
#define XOS_IS_LOGGED_WARN  XOS_IS_LOGGED_PLAIN_WARN
#define XOS_IS_LOGGED_INFO  XOS_IS_LOGGED_PLAIN_INFO
#define XOS_IS_LOGGED_DEBUG XOS_IS_LOGGED_PLAIN_DEBUG
#define XOS_IS_LOGGED_TRACE XOS_IS_LOGGED_PLAIN_TRACE
#else // defined(XOS_PLAIN_LOGGING)
#if defined(XOS_FUNCTION_LOGGING)
#define XOS_IS_LOGGED_FATAL XOS_IS_LOGGED_FUNCTION_FATAL
#define XOS_IS_LOGGED_ERROR XOS_IS_LOGGED_FUNCTION_ERROR
#define XOS_IS_LOGGED_WARN  XOS_IS_LOGGED_FUNCTION_WARN
#define XOS_IS_LOGGED_INFO  XOS_IS_LOGGED_FUNCTION_INFO
#define XOS_IS_LOGGED_DEBUG XOS_IS_LOGGED_FUNCTION_DEBUG
#define XOS_IS_LOGGED_TRACE XOS_IS_LOGGED_FUNCTION_TRACE
#else // defined(XOS_FUNCTION_LOGGING)
#define XOS_IS_LOGGED_FATAL XOS_IS_LOGGED_LOCATION_FATAL
#define XOS_IS_LOGGED_ERROR XOS_IS_LOGGED_LOCATION_ERROR
#define XOS_IS_LOGGED_WARN  XOS_IS_LOGGED_LOCATION_WARN
#define XOS_IS_LOGGED_INFO  XOS_IS_LOGGED_LOCATION_INFO
#define XOS_IS_LOGGED_DEBUG XOS_IS_LOGGED_LOCATION_DEBUG
#define XOS_IS_LOGGED_TRACE XOS_IS_LOGGED_LOCATION_TRACE
#endif // defined(XOS_FUNCTION_LOGGING)
#endif // defined(XOS_PLAIN_LOGGING)

///
///
///
#define XOS_LOG_LOCATION_MESSAGE_FATAL(message) XOS_LOG_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_FATAL, message)
#define XOS_LOG_LOCATION_MESSAGE_ERROR(message) XOS_LOG_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_ERROR, message)
#define XOS_LOG_LOCATION_MESSAGE_WARN(message) XOS_LOG_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_WARN, message)
#define XOS_LOG_LOCATION_MESSAGE_INFO(message) XOS_LOG_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_INFO, message)
#define XOS_LOG_LOCATION_MESSAGE_DEBUG(message) XOS_LOG_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_DEBUG, message)
#define XOS_LOG_LOCATION_MESSAGE_TRACE(message) XOS_LOG_LOCATION(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_TRACE, message)

#define XOS_LOG_FUNCTION_MESSAGE_FATAL(message) XOS_LOG_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_FATAL, message)
#define XOS_LOG_FUNCTION_MESSAGE_ERROR(message) XOS_LOG_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_ERROR, message)
#define XOS_LOG_FUNCTION_MESSAGE_WARN(message) XOS_LOG_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_WARN, message)
#define XOS_LOG_FUNCTION_MESSAGE_INFO(message) XOS_LOG_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_INFO, message)
#define XOS_LOG_FUNCTION_MESSAGE_DEBUG(message) XOS_LOG_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_DEBUG, message)
#define XOS_LOG_FUNCTION_MESSAGE_TRACE(message) XOS_LOG_FUNCTION(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_TRACE, message)

#define XOS_LOG_PLAIN_MESSAGE_FATAL(message) XOS_LOG_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_FATAL, message)
#define XOS_LOG_PLAIN_MESSAGE_ERROR(message) XOS_LOG_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_ERROR, message)
#define XOS_LOG_PLAIN_MESSAGE_WARN(message) XOS_LOG_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_WARN, message)
#define XOS_LOG_PLAIN_MESSAGE_INFO(message) XOS_LOG_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_INFO, message)
#define XOS_LOG_PLAIN_MESSAGE_DEBUG(message) XOS_LOG_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_DEBUG, message)
#define XOS_LOG_PLAIN_MESSAGE_TRACE(message) XOS_LOG_PLAIN(XOS_DEFAULT_LOGGER, XOS_LOGGING_MESSAGE_LEVEL_TRACE, message)

#if defined(XOS_PLAIN_MESSAGE_LOGGING)
#define XOS_LOG_MESSAGE_FATAL XOS_LOG_PLAIN_MESSAGE_FATAL
#define XOS_LOG_MESSAGE_ERROR XOS_LOG_PLAIN_MESSAGE_ERROR
#define XOS_LOG_MESSAGE_WARN  XOS_LOG_PLAIN_MESSAGE_WARN
#define XOS_LOG_MESSAGE_INFO  XOS_LOG_PLAIN_MESSAGE_INFO
#define XOS_LOG_MESSAGE_DEBUG XOS_LOG_PLAIN_MESSAGE_DEBUG
#define XOS_LOG_MESSAGE_TRACE XOS_LOG_PLAIN_MESSAGE_TRACE
#else // defined(XOS_PLAIN_MESSAGE_LOGGING)
#if defined(XOS_LOCATION_MESSAGE_LOGGING)
#define XOS_LOG_MESSAGE_FATAL XOS_LOG_LOCATION_MESSAGE_FATAL
#define XOS_LOG_MESSAGE_ERROR XOS_LOG_LOCATION_MESSAGE_ERROR
#define XOS_LOG_MESSAGE_WARN  XOS_LOG_LOCATION_MESSAGE_WARN
#define XOS_LOG_MESSAGE_INFO  XOS_LOG_LOCATION_MESSAGE_INFO
#define XOS_LOG_MESSAGE_DEBUG XOS_LOG_LOCATION_MESSAGE_DEBUG
#define XOS_LOG_MESSAGE_TRACE XOS_LOG_LOCATION_MESSAGE_TRACE
#else // defined(XOS_LOCATION_MESSAGE_LOGGING)
#define XOS_LOG_MESSAGE_FATAL XOS_LOG_FUNCTION_MESSAGE_FATAL
#define XOS_LOG_MESSAGE_ERROR XOS_LOG_FUNCTION_MESSAGE_ERROR
#define XOS_LOG_MESSAGE_WARN  XOS_LOG_FUNCTION_MESSAGE_WARN
#define XOS_LOG_MESSAGE_INFO  XOS_LOG_FUNCTION_MESSAGE_INFO
#define XOS_LOG_MESSAGE_DEBUG XOS_LOG_FUNCTION_MESSAGE_DEBUG
#define XOS_LOG_MESSAGE_TRACE XOS_LOG_FUNCTION_MESSAGE_TRACE
#endif // defined(XOS_LOCATION_MESSAGE_LOGGING)
#endif // defined(XOS_PLAIN_MESSAGE_LOGGING)

///
/// 
///
#define XOS_IF_ERR_LOGGED_LOCATION_FATAL(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_LOCATION_FATAL(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_LOCATION_FATAL(message); } }

#define XOS_IF_ERR_LOGGED_LOCATION_ERROR(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_LOCATION_ERROR(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_LOCATION_ERROR(message); } }

#define XOS_IF_ERR_LOGGED_LOCATION_WARN(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_LOCATION_WARN(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_LOCATION_WARN(message); } }

#define XOS_IF_ERR_LOGGED_LOCATION_INFO(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_LOCATION_INFO(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_LOCATION_INFO(message); } }

#define XOS_IF_ERR_LOGGED_LOCATION_DEBUG(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_LOCATION_DEBUG(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_LOCATION_DEBUG(message); } }

#define XOS_IF_ERR_LOGGED_LOCATION_TRACE(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_LOCATION_TRACE(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_LOCATION_TRACE(message); } }


#define XOS_IF_ERR_LOGGED_FUNCTION_FATAL(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_FUNCTION_FATAL(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_FUNCTION_FATAL(message); } }

#define XOS_IF_ERR_LOGGED_FUNCTION_ERROR(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_FUNCTION_ERROR(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_FUNCTION_ERROR(message); } }

#define XOS_IF_ERR_LOGGED_FUNCTION_WARN(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_FUNCTION_WARN(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_FUNCTION_WARN(message); } }

#define XOS_IF_ERR_LOGGED_FUNCTION_INFO(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_FUNCTION_INFO(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_FUNCTION_INFO(message); } }

#define XOS_IF_ERR_LOGGED_FUNCTION_DEBUG(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_FUNCTION_DEBUG(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_FUNCTION_DEBUG(message); } }

#define XOS_IF_ERR_LOGGED_FUNCTION_TRACE(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_FUNCTION_TRACE(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_FUNCTION_TRACE(message); } }


#define XOS_IF_ERR_LOGGED_PLAIN_FATAL(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_PLAIN_FATAL(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_PLAIN_FATAL(message); } }

#define XOS_IF_ERR_LOGGED_PLAIN_ERROR(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_PLAIN_ERROR(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_PLAIN_ERROR(message); } }

#define XOS_IF_ERR_LOGGED_PLAIN_WARN(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_PLAIN_WARN(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_PLAIN_WARN(message); } }

#define XOS_IF_ERR_LOGGED_PLAIN_INFO(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_PLAIN_INFO(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_PLAIN_INFO(message); } }

#define XOS_IF_ERR_LOGGED_PLAIN_DEBUG(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_PLAIN_DEBUG(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_PLAIN_DEBUG(message); } }

#define XOS_IF_ERR_LOGGED_PLAIN_TRACE(isLogged, isErrLogged, message) \
if (isLogged) { XOS_LOG_PLAIN_TRACE(message); } \
else { if (isErrLogged) { XOS_ERR_LOG_PLAIN_TRACE(message); } }

///
/// 
///
#define XOS_IS_ERR_LOGGED_LOCATION_FATAL(message) \
XOS_IF_ERR_LOGGED_LOCATION_FATAL(this->IsLogged(), this->IsErrLogged(), message)

#define XOS_IS_ERR_LOGGED_LOCATION_ERROR(message) \
XOS_IF_ERR_LOGGED_LOCATION_ERROR(this->IsLogged(), this->IsErrLogged(), message)

#define XOS_IS_ERR_LOGGED_LOCATION_WARN(message) \
XOS_IF_ERR_LOGGED_LOCATION_WARN(this->IsLogged(), this->IsErrLogged(), message)

#define XOS_IS_ERR_LOGGED_LOCATION_INFO(message) \
XOS_IF_ERR_LOGGED_LOCATION_INFO(this->IsLogged(), this->IsErrLogged(), message)

#define XOS_IS_ERR_LOGGED_LOCATION_DEBUG(message) \
XOS_IF_ERR_LOGGED_LOCATION_DEBUG(this->IsLogged(), this->IsErrLogged(), message)

#define XOS_IS_ERR_LOGGED_LOCATION_TRACE(message) \
XOS_IF_ERR_LOGGED_LOCATION_TRACE(this->IsLogged(), this->IsErrLogged(), message)


#define XOS_IS_ERR_LOGGED_FUNCTION_FATAL(message) \
XOS_IF_ERR_LOGGED_FUNCTION_FATAL(this->IsLogged(), this->IsErrLogged(), message)

#define XOS_IS_ERR_LOGGED_FUNCTION_ERROR(message) \
XOS_IF_ERR_LOGGED_FUNCTION_ERROR(this->IsLogged(), this->IsErrLogged(), message)

#define XOS_IS_ERR_LOGGED_FUNCTION_WARN(message) \
XOS_IF_ERR_LOGGED_FUNCTION_WARN(this->IsLogged(), this->IsErrLogged(), message)

#define XOS_IS_ERR_LOGGED_FUNCTION_INFO(message) \
XOS_IF_ERR_LOGGED_FUNCTION_INFO(this->IsLogged(), this->IsErrLogged(), message)

#define XOS_IS_ERR_LOGGED_FUNCTION_DEBUG(message) \
XOS_IF_ERR_LOGGED_FUNCTION_DEBUG(this->IsLogged(), this->IsErrLogged(), message)

#define XOS_IS_ERR_LOGGED_FUNCTION_TRACE(message) \
XOS_IF_ERR_LOGGED_FUNCTION_TRACE(this->IsLogged(), this->IsErrLogged(), message)


#define XOS_IS_ERR_LOGGED_PLAIN_FATAL(message) \
XOS_IF_ERR_LOGGED_PLAIN_FATAL(this->IsLogged(), this->IsErrLogged(), message)

#define XOS_IS_ERR_LOGGED_PLAIN_ERROR(message) \
XOS_IF_ERR_LOGGED_PLAIN_ERROR(this->IsLogged(), this->IsErrLogged(), message)

#define XOS_IS_ERR_LOGGED_PLAIN_WARN(message) \
XOS_IF_ERR_LOGGED_PLAIN_WARN(this->IsLogged(), this->IsErrLogged(), message)

#define XOS_IS_ERR_LOGGED_PLAIN_INFO(message) \
XOS_IF_ERR_LOGGED_PLAIN_INFO(this->IsLogged(), this->IsErrLogged(), message)

#define XOS_IS_ERR_LOGGED_PLAIN_DEBUG(message) \
XOS_IF_ERR_LOGGED_PLAIN_DEBUG(this->IsLogged(), this->IsErrLogged(), message)

#define XOS_IS_ERR_LOGGED_PLAIN_TRACE(message) \
XOS_IF_ERR_LOGGED_PLAIN_TRACE(this->IsLogged(), this->IsErrLogged(), message)

///
///
///
#if defined(XOS_PLAIN_LOGGING)
#define XOS_IF_ERR_LOGGED_FATAL XOS_IF_ERR_LOGGED_PLAIN_FATAL
#define XOS_IF_ERR_LOGGED_ERROR XOS_IF_ERR_LOGGED_PLAIN_ERROR
#define XOS_IF_ERR_LOGGED_WARN  XOS_IF_ERR_LOGGED_PLAIN_WARN
#define XOS_IF_ERR_LOGGED_INFO  XOS_IF_ERR_LOGGED_PLAIN_INFO
#define XOS_IF_ERR_LOGGED_DEBUG XOS_IF_ERR_LOGGED_PLAIN_DEBUG
#define XOS_IF_ERR_LOGGED_TRACE XOS_IF_ERR_LOGGED_PLAIN_TRACE
#else // defined(XOS_PLAIN_LOGGING)
#if defined(XOS_FUNCTION_LOGGING)
#define XOS_IF_ERR_LOGGED_FATAL XOS_IF_ERR_LOGGED_FUNCTION_FATAL
#define XOS_IF_ERR_LOGGED_ERROR XOS_IF_ERR_LOGGED_FUNCTION_ERROR
#define XOS_IF_ERR_LOGGED_WARN  XOS_IF_ERR_LOGGED_FUNCTION_WARN
#define XOS_IF_ERR_LOGGED_INFO  XOS_IF_ERR_LOGGED_FUNCTION_INFO
#define XOS_IF_ERR_LOGGED_DEBUG XOS_IF_ERR_LOGGED_FUNCTION_DEBUG
#define XOS_IF_ERR_LOGGED_TRACE XOS_IF_ERR_LOGGED_FUNCTION_TRACE
#else // defined(XOS_FUNCTION_LOGGING)
#define XOS_IF_ERR_LOGGED_FATAL XOS_IF_ERR_LOGGED_LOCATION_FATAL
#define XOS_IF_ERR_LOGGED_ERROR XOS_IF_ERR_LOGGED_LOCATION_ERROR
#define XOS_IF_ERR_LOGGED_WARN  XOS_IF_ERR_LOGGED_LOCATION_WARN
#define XOS_IF_ERR_LOGGED_INFO  XOS_IF_ERR_LOGGED_LOCATION_INFO
#define XOS_IF_ERR_LOGGED_DEBUG XOS_IF_ERR_LOGGED_LOCATION_DEBUG
#define XOS_IF_ERR_LOGGED_TRACE XOS_IF_ERR_LOGGED_LOCATION_TRACE
#endif // defined(XOS_FUNCTION_LOGGING)
#endif // defined(XOS_PLAIN_LOGGING)

///
///
///
#if defined(XOS_PLAIN_LOGGING)
#define XOS_IS_ERR_LOGGED_FATAL XOS_IS_ERR_LOGGED_PLAIN_FATAL
#define XOS_IS_ERR_LOGGED_ERROR XOS_IS_ERR_LOGGED_PLAIN_ERROR
#define XOS_IS_ERR_LOGGED_WARN  XOS_IS_ERR_LOGGED_PLAIN_WARN
#define XOS_IS_ERR_LOGGED_INFO  XOS_IS_ERR_LOGGED_PLAIN_INFO
#define XOS_IS_ERR_LOGGED_DEBUG XOS_IS_ERR_LOGGED_PLAIN_DEBUG
#define XOS_IS_ERR_LOGGED_TRACE XOS_IS_ERR_LOGGED_PLAIN_TRACE
#else // defined(XOS_PLAIN_LOGGING)
#if defined(XOS_FUNCTION_LOGGING)
#define XOS_IS_ERR_LOGGED_FATAL XOS_IS_ERR_LOGGED_FUNCTION_FATAL
#define XOS_IS_ERR_LOGGED_ERROR XOS_IS_ERR_LOGGED_FUNCTION_ERROR
#define XOS_IS_ERR_LOGGED_WARN  XOS_IS_ERR_LOGGED_FUNCTION_WARN
#define XOS_IS_ERR_LOGGED_INFO  XOS_IS_ERR_LOGGED_FUNCTION_INFO
#define XOS_IS_ERR_LOGGED_DEBUG XOS_IS_ERR_LOGGED_FUNCTION_DEBUG
#define XOS_IS_ERR_LOGGED_TRACE XOS_IS_ERR_LOGGED_FUNCTION_TRACE
#else // defined(XOS_FUNCTION_LOGGING)
#define XOS_IS_ERR_LOGGED_FATAL XOS_IS_ERR_LOGGED_LOCATION_FATAL
#define XOS_IS_ERR_LOGGED_ERROR XOS_IS_ERR_LOGGED_LOCATION_ERROR
#define XOS_IS_ERR_LOGGED_WARN  XOS_IS_ERR_LOGGED_LOCATION_WARN
#define XOS_IS_ERR_LOGGED_INFO  XOS_IS_ERR_LOGGED_LOCATION_INFO
#define XOS_IS_ERR_LOGGED_DEBUG XOS_IS_ERR_LOGGED_LOCATION_DEBUG
#define XOS_IS_ERR_LOGGED_TRACE XOS_IS_ERR_LOGGED_LOCATION_TRACE
#endif // defined(XOS_FUNCTION_LOGGING)
#endif // defined(XOS_PLAIN_LOGGING)

///
///
///
#if !defined(LOG_DEBUG_TRACE)
#define LOG_DEBUG_TRACE(condition_, message_) XOS_LOG_DEBUG_TRACE(condition_, message_)
#define LOG_PLAIN_DEBUG_TRACE(condition_, message_) XOS_LOG_PLAIN_DEBUG_TRACE(condition_, message_)
#define LOG_FUNCTION_DEBUG_TRACE(condition_, message_) XOS_LOG_FUNCTION_DEBUG_TRACE(condition_, message_)
#define LOG_LOCATION_DEBUG_TRACE(condition_, message_) XOS_LOG_LOCATION_DEBUG_TRACE(condition_, message_)
#endif /// !defined(LOG_DEBUG_TRACE)

#if !defined(LOG_TRACE)
#define LOG_TRACE(message_) XOS_LOG_TRACE(message_)
#define LOG_PLAIN_TRACE(message_) XOS_LOG_PLAIN_TRACE(message_)
#define LOG_FUNCTION_TRACE(message_) XOS_LOG_FUNCTION_TRACE(message_)
#define LOG_LOCATION_TRACE(message_) XOS_LOG_LOCATION_TRACE(message_)
#define LOG_MESSAGE_TRACE(message_) XOS_LOG_MESSAGE_TRACE(message_)
#define LOG_PLAIN_MESSAGE_TRACE(message_) XOS_LOG_PLAIN_MESSAGE_TRACE(message_)
#define LOG_FUNCTION_MESSAGE_TRACE(message_) XOS_LOG_FUNCTION_MESSAGE_TRACE(message_)
#define LOG_LOCATION_MESSAGE_TRACE(message_) XOS_LOG_LOCATION_MESSAGE_TRACE(message_)
#endif /// !defined(LOG_TRACE)

#if !defined(LOG_DEBUG)
#define LOG_DEBUG(message_) XOS_LOG_DEBUG(message_)
#define LOG_PLAIN_DEBUG(message_) XOS_LOG_PLAIN_DEBUG(message_)
#define LOG_FUNCTION_DEBUG(message_) XOS_LOG_FUNCTION_DEBUG(message_)
#define LOG_LOCATION_DEBUG(message_) XOS_LOG_LOCATION_DEBUG(message_)
#define LOG_MESSAGE_DEBUG(message_) XOS_LOG_MESSAGE_DEBUG(message_)
#define LOG_PLAIN_MESSAGE_DEBUG(message_) XOS_LOG_PLAIN_MESSAGE_DEBUG(message_)
#define LOG_FUNCTION_MESSAGE_DEBUG(message_) XOS_LOG_FUNCTION_MESSAGE_DEBUG(message_)
#define LOG_LOCATION_MESSAGE_DEBUG(message_) XOS_LOG_LOCATION_MESSAGE_DEBUG(message_)
#endif /// !defined(LOG_DEBUG)

#if !defined(LOG_INFO)
#define LOG_INFO(message_) XOS_LOG_INFO(message_)
#define LOG_PLAIN_INFO(message_) XOS_LOG_PLAIN_INFO(message_)
#define LOG_FUNCTION_INFO(message_) XOS_LOG_FUNCTION_INFO(message_)
#define LOG_LOCATION_INFO(message_) XOS_LOG_LOCATION_INFO(message_)
#define LOG_MESSAGE_INFO(message_) XOS_LOG_MESSAGE_INFO(message_)
#define LOG_PLAIN_MESSAGE_INFO(message_) XOS_LOG_PLAIN_MESSAGE_INFO(message_)
#define LOG_FUNCTION_MESSAGE_INFO(message_) XOS_LOG_FUNCTION_MESSAGE_INFO(message_)
#define LOG_LOCATION_MESSAGE_INFO(message_) XOS_LOG_LOCATION_MESSAGE_INFO(message_)
#endif /// !defined(LOG_INFO)

#if !defined(LOG_WARN)
#define LOG_WARN(message_) XOS_LOG_WARN(message_)
#define LOG_PLAIN_WARN(message_) XOS_LOG_PLAIN_WARN(message_)
#define LOG_FUNCTION_WARN(message_) XOS_LOG_FUNCTION_WARN(message_)
#define LOG_LOCATION_WARN(message_) XOS_LOG_LOCATION_WARN(message_)
#define LOG_MESSAGE_WARN(message_) XOS_LOG_MESSAGE_WARN(message_)
#define LOG_PLAIN_MESSAGE_WARN(message_) XOS_LOG_PLAIN_MESSAGE_WARN(message_)
#define LOG_FUNCTION_MESSAGE_WARN(message_) XOS_LOG_FUNCTION_MESSAGE_WARN(message_)
#define LOG_LOCATION_MESSAGE_WARN(message_) XOS_LOG_LOCATION_MESSAGE_WARN(message_)
#endif /// !defined(LOG_WARN)

#if !defined(LOG_ERROR)
#define LOG_ERROR(message_) XOS_LOG_ERROR(message_)
#define LOG_PLAIN_ERROR(message_) XOS_LOG_PLAIN_ERROR(message_)
#define LOG_FUNCTION_ERROR(message_) XOS_LOG_FUNCTION_ERROR(message_)
#define LOG_LOCATION_ERROR(message_) XOS_LOG_LOCATION_ERROR(message_)
#define LOG_MESSAGE_ERROR(message_) XOS_LOG_MESSAGE_ERROR(message_)
#define LOG_PLAIN_MESSAGE_ERROR(message_) XOS_LOG_PLAIN_MESSAGE_ERROR(message_)
#define LOG_FUNCTION_MESSAGE_ERROR(message_) XOS_LOG_FUNCTION_MESSAGE_ERROR(message_)
#define LOG_LOCATION_MESSAGE_ERROR(message_) XOS_LOG_LOCATION_MESSAGE_ERROR(message_)
#endif /// !defined(LOG_ERROR)

#if !defined(LOG_FATAL)
#define LOG_FATAL(message_) XOS_LOG_FATAL(message_)
#define LOG_PLAIN_FATAL(message_) XOS_LOG_PLAIN_FATAL(message_)
#define LOG_FUNCTION_FATAL(message_) XOS_LOG_FUNCTION_FATAL(message_)
#define LOG_LOCATION_FATAL(message_) XOS_LOG_LOCATION_FATAL(message_)
#define LOG_MESSAGE_FATAL(message_) XOS_LOG_MESSAGE_FATAL(message_)
#define LOG_PLAIN_MESSAGE_FATAL(message_) XOS_LOG_PLAIN_MESSAGE_FATAL(message_)
#define LOG_FUNCTION_MESSAGE_FATAL(message_) XOS_LOG_FUNCTION_MESSAGE_FATAL(message_)
#define LOG_LOCATION_MESSAGE_FATAL(message_) XOS_LOG_LOCATION_MESSAGE_FATAL(message_)
#endif /// !defined(LOG_FATAL)

///
///
///
#if !defined(IF_LOGGED_TRACE)
#define IF_LOGGED_TRACE(isLogged_, message_) XOS_IF_LOGGED_TRACE(isLogged_, message_)
#define IF_LOGGED_PLAIN_TRACE(isLogged_, message_) XOS_IF_LOGGED_PLAIN_TRACE(isLogged_, message_)
#define IF_LOGGED_FUNCTION_TRACE(isLogged_, message_) XOS_IF_LOGGED_FUNCTION_TRACE(isLogged_, message_)
#define IF_LOGGED_LOCATION_TRACE(isLogged_, message_) XOS_IF_LOGGED_LOCATION_TRACE(isLogged_, message_)
#endif /// !defined(IF_LOGGED_TRACE)

#if !defined(IF_LOGGED_DEBUG)
#define IF_LOGGED_DEBUG(isLogged_, message_) XOS_IF_LOGGED_DEBUG(isLogged_, message_)
#define IF_LOGGED_PLAIN_DEBUG(isLogged_, message_) XOS_IF_LOGGED_PLAIN_DEBUG(isLogged_, message_)
#define IF_LOGGED_FUNCTION_DEBUG(isLogged_, message_) XOS_IF_LOGGED_FUNCTION_DEBUG(isLogged_, message_)
#define IF_LOGGED_LOCATION_DEBUG(isLogged_, message_) XOS_IF_LOGGED_LOCATION_DEBUG(isLogged_, message_)
#endif /// !defined(IF_LOGGED_DEBUG)

#if !defined(IF_LOGGED_INFO)
#define IF_LOGGED_INFO(isLogged_, message_) XOS_IF_LOGGED_INFO(isLogged_, message_)
#define IF_LOGGED_PLAIN_INFO(isLogged_, message_) XOS_IF_LOGGED_PLAIN_INFO(isLogged_, message_)
#define IF_LOGGED_FUNCTION_INFO(isLogged_, message_) XOS_IF_LOGGED_FUNCTION_INFO(isLogged_, message_)
#define IF_LOGGED_LOCATION_INFO(isLogged_, message_) XOS_IF_LOGGED_LOCATION_INFO(isLogged_, message_)
#endif /// !defined(IF_LOGGED_INFO)

#if !defined(IF_LOGGED_WARN)
#define IF_LOGGED_WARN(isLogged_, message_) XOS_IF_LOGGED_WARN(isLogged_, message_)
#define IF_LOGGED_PLAIN_WARN(isLogged_, message_) XOS_IF_LOGGED_PLAIN_WARN(isLogged_, message_)
#define IF_LOGGED_FUNCTION_WARN(isLogged_, message_) XOS_IF_LOGGED_FUNCTION_WARN(isLogged_, message_)
#define IF_LOGGED_LOCATION_WARN(isLogged_, message_) XOS_IF_LOGGED_LOCATION_WARN(isLogged_, message_)
#endif /// !defined(IF_LOGGED_WARN)

#if !defined(IF_LOGGED_ERROR)
#define IF_LOGGED_ERROR(isLogged_, message_) XOS_IF_LOGGED_ERROR(isLogged_, message_)
#define IF_LOGGED_PLAIN_ERROR(isLogged_, message_) XOS_IF_LOGGED_PLAIN_ERROR(isLogged_, message_)
#define IF_LOGGED_FUNCTION_ERROR(isLogged_, message_) XOS_IF_LOGGED_FUNCTION_ERROR(isLogged_, message_)
#define IF_LOGGED_LOCATION_ERROR(isLogged_, message_) XOS_IF_LOGGED_LOCATION_ERROR(isLogged_, message_)
#endif /// !defined(IF_LOGGED_ERROR)

#if !defined(IF_LOGGED_FATAL)
#define IF_LOGGED_FATAL(isLogged_, message_) XOS_IF_LOGGED_FATAL(isLogged_, message_)
#define IF_LOGGED_PLAIN_FATAL(isLogged_, message_) XOS_IF_LOGGED_PLAIN_FATAL(isLogged_, message_)
#define IF_LOGGED_FUNCTION_FATAL(isLogged_, message_) XOS_IF_LOGGED_FUNCTION_FATAL(isLogged_, message_)
#define IF_LOGGED_LOCATION_FATAL(isLogged_, message_) XOS_IF_LOGGED_LOCATION_FATAL(isLogged_, message_)
#endif /// !defined(LOG_FATAL)

///
///
///
#if !defined(IF_ERR_LOGGED_TRACE)
#define IF_ERR_LOGGED_TRACE(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_TRACE(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_PLAIN_TRACE(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_PLAIN_TRACE(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_FUNCTION_TRACE(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_FUNCTION_TRACE(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_LOCATION_TRACE(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_LOCATION_TRACE(isLogged_, isErrLogged_, message_)
#endif /// !defined(IF_ERR_LOGGED_TRACE)

#if !defined(IF_ERR_LOGGED_DEBUG)
#define IF_ERR_LOGGED_DEBUG(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_DEBUG(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_PLAIN_DEBUG(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_PLAIN_DEBUG(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_FUNCTION_DEBUG(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_FUNCTION_DEBUG(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_LOCATION_DEBUG(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_LOCATION_DEBUG(isLogged_, isErrLogged_, message_)
#endif /// !defined(IF_ERR_LOGGED_DEBUG)

#if !defined(IF_ERR_LOGGED_INFO)
#define IF_ERR_LOGGED_INFO(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_INFO(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_PLAIN_INFO(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_PLAIN_INFO(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_FUNCTION_INFO(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_FUNCTION_INFO(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_LOCATION_INFO(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_LOCATION_INFO(isLogged_, isErrLogged_, message_)
#endif /// !defined(IF_ERR_LOGGED_INFO)

#if !defined(IF_ERR_LOGGED_WARN)
#define IF_ERR_LOGGED_WARN(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_WARN(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_PLAIN_WARN(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_PLAIN_WARN(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_FUNCTION_WARN(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_FUNCTION_WARN(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_LOCATION_WARN(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_LOCATION_WARN(isLogged_, isErrLogged_, message_)
#endif /// !defined(IF_ERR_LOGGED_WARN)

#if !defined(IF_ERR_LOGGED_ERROR)
#define IF_ERR_LOGGED_ERROR(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_ERROR(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_PLAIN_ERROR(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_PLAIN_ERROR(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_FUNCTION_ERROR(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_FUNCTION_ERROR(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_LOCATION_ERROR(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_LOCATION_ERROR(isLogged_, isErrLogged_, message_)
#endif /// !defined(IF_ERR_LOGGED_ERROR)

#if !defined(IF_ERR_LOGGED_FATAL)
#define IF_ERR_LOGGED_FATAL(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_FATAL(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_PLAIN_FATAL(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_PLAIN_FATAL(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_FUNCTION_FATAL(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_FUNCTION_FATAL(isLogged_, isErrLogged_, message_)
#define IF_ERR_LOGGED_LOCATION_FATAL(isLogged_, isErrLogged_, message_) XOS_IF_ERR_LOGGED_LOCATION_FATAL(isLogged_, isErrLogged_, message_)
#endif /// !defined(LOG_FATAL)

///
///
///
#if !defined(IS_LOGGED_TRACE)
#define IS_LOGGED_TRACE(message_) XOS_IS_LOGGED_TRACE(message_)
#define IS_LOGGED_PLAIN_TRACE(message_) XOS_IS_LOGGED_PLAIN_TRACE(message_)
#define IS_LOGGED_FUNCTION_TRACE(message_) XOS_IS_LOGGED_FUNCTION_TRACE(message_)
#define IS_LOGGED_LOCATION_TRACE(message_) XOS_IS_LOGGED_LOCATION_TRACE(message_)
#endif /// !defined(IS_LOGGED_TRACE)

#if !defined(IS_LOGGED_DEBUG)
#define IS_LOGGED_DEBUG(message_) XOS_IS_LOGGED_DEBUG(message_)
#define IS_LOGGED_PLAIN_DEBUG(message_) XOS_IS_LOGGED_PLAIN_DEBUG(message_)
#define IS_LOGGED_FUNCTION_DEBUG(message_) XOS_IS_LOGGED_FUNCTION_DEBUG(message_)
#define IS_LOGGED_LOCATION_DEBUG(message_) XOS_IS_LOGGED_LOCATION_DEBUG(message_)
#endif /// !defined(IS_LOGGED_DEBUG)

#if !defined(IS_LOGGED_INFO)
#define IS_LOGGED_INFO(message_) XOS_IS_LOGGED_INFO(message_)
#define IS_LOGGED_PLAIN_INFO(message_) XOS_IS_LOGGED_PLAIN_INFO(message_)
#define IS_LOGGED_FUNCTION_INFO(message_) XOS_IS_LOGGED_FUNCTION_INFO(message_)
#define IS_LOGGED_LOCATION_INFO(message_) XOS_IS_LOGGED_LOCATION_INFO(message_)
#endif /// !defined(IS_LOGGED_INFO)

#if !defined(IS_LOGGED_WARN)
#define IS_LOGGED_WARN(message_) XOS_IS_LOGGED_WARN(message_)
#define IS_LOGGED_PLAIN_WARN(message_) XOS_IS_LOGGED_PLAIN_WARN(message_)
#define IS_LOGGED_FUNCTION_WARN(message_) XOS_IS_LOGGED_FUNCTION_WARN(message_)
#define IS_LOGGED_LOCATION_WARN(message_) XOS_IS_LOGGED_LOCATION_WARN(message_)
#endif /// !defined(IS_LOGGED_WARN)

#if !defined(IS_LOGGED_ERROR)
#define IS_LOGGED_ERROR(message_) XOS_IS_LOGGED_ERROR(message_)
#define IS_LOGGED_PLAIN_ERROR(message_) XOS_IS_LOGGED_PLAIN_ERROR(message_)
#define IS_LOGGED_FUNCTION_ERROR(message_) XOS_IS_LOGGED_FUNCTION_ERROR(message_)
#define IS_LOGGED_LOCATION_ERROR(message_) XOS_IS_LOGGED_LOCATION_ERROR(message_)
#endif /// !defined(IS_LOGGED_ERROR)

#if !defined(IS_LOGGED_FATAL)
#define IS_LOGGED_FATAL(message_) XOS_IS_LOGGED_FATAL(message_)
#define IS_LOGGED_PLAIN_FATAL(message_) XOS_IS_LOGGED_PLAIN_FATAL(message_)
#define IS_LOGGED_FUNCTION_FATAL(message_) XOS_IS_LOGGED_FUNCTION_FATAL(message_)
#define IS_LOGGED_LOCATION_FATAL(message_) XOS_IS_LOGGED_LOCATION_FATAL(message_)
#endif /// !defined(LOG_FATAL)

///
///
///
#if !defined(IS_ERR_LOGGED_TRACE)
#define IS_ERR_LOGGED_TRACE(message_) XOS_IS_ERR_LOGGED_TRACE(message_)
#define IS_ERR_LOGGED_PLAIN_TRACE(message_) XOS_IS_ERR_LOGGED_PLAIN_TRACE(message_)
#define IS_ERR_LOGGED_FUNCTION_TRACE(message_) XOS_IS_ERR_LOGGED_FUNCTION_TRACE(message_)
#define IS_ERR_LOGGED_LOCATION_TRACE(message_) XOS_IS_ERR_LOGGED_LOCATION_TRACE(message_)
#endif /// !defined(IS_ERR_LOGGED_TRACE)

#if !defined(IS_ERR_LOGGED_DEBUG)
#define IS_ERR_LOGGED_DEBUG(message_) XOS_IS_ERR_LOGGED_DEBUG(message_)
#define IS_ERR_LOGGED_PLAIN_DEBUG(message_) XOS_IS_ERR_LOGGED_PLAIN_DEBUG(message_)
#define IS_ERR_LOGGED_FUNCTION_DEBUG(message_) XOS_IS_ERR_LOGGED_FUNCTION_DEBUG(message_)
#define IS_ERR_LOGGED_LOCATION_DEBUG(message_) XOS_IS_ERR_LOGGED_LOCATION_DEBUG(message_)
#endif /// !defined(IS_ERR_LOGGED_DEBUG)

#if !defined(IS_ERR_LOGGED_INFO)
#define IS_ERR_LOGGED_INFO(message_) XOS_IS_ERR_LOGGED_INFO(message_)
#define IS_ERR_LOGGED_PLAIN_INFO(message_) XOS_IS_ERR_LOGGED_PLAIN_INFO(message_)
#define IS_ERR_LOGGED_FUNCTION_INFO(message_) XOS_IS_ERR_LOGGED_FUNCTION_INFO(message_)
#define IS_ERR_LOGGED_LOCATION_INFO(message_) XOS_IS_ERR_LOGGED_LOCATION_INFO(message_)
#endif /// !defined(IS_ERR_LOGGED_INFO)

#if !defined(IS_ERR_LOGGED_WARN)
#define IS_ERR_LOGGED_WARN(message_) XOS_IS_ERR_LOGGED_WARN(message_)
#define IS_ERR_LOGGED_PLAIN_WARN(message_) XOS_IS_ERR_LOGGED_PLAIN_WARN(message_)
#define IS_ERR_LOGGED_FUNCTION_WARN(message_) XOS_IS_ERR_LOGGED_FUNCTION_WARN(message_)
#define IS_ERR_LOGGED_LOCATION_WARN(message_) XOS_IS_ERR_LOGGED_LOCATION_WARN(message_)
#endif /// !defined(IS_ERR_LOGGED_WARN)

#if !defined(IS_ERR_LOGGED_ERROR)
#define IS_ERR_LOGGED_ERROR(message_) XOS_IS_ERR_LOGGED_ERROR(message_)
#define IS_ERR_LOGGED_PLAIN_ERROR(message_) XOS_IS_ERR_LOGGED_PLAIN_ERROR(message_)
#define IS_ERR_LOGGED_FUNCTION_ERROR(message_) XOS_IS_ERR_LOGGED_FUNCTION_ERROR(message_)
#define IS_ERR_LOGGED_LOCATION_ERROR(message_) XOS_IS_ERR_LOGGED_LOCATION_ERROR(message_)
#endif /// !defined(IS_ERR_LOGGED_ERROR)

#if !defined(IS_ERR_LOGGED_FATAL)
#define IS_ERR_LOGGED_FATAL(message_) XOS_IS_ERR_LOGGED_FATAL(message_)
#define IS_ERR_LOGGED_PLAIN_FATAL(message_) XOS_IS_ERR_LOGGED_PLAIN_FATAL(message_)
#define IS_ERR_LOGGED_FUNCTION_FATAL(message_) XOS_IS_ERR_LOGGED_FUNCTION_FATAL(message_)
#define IS_ERR_LOGGED_LOCATION_FATAL(message_) XOS_IS_ERR_LOGGED_LOCATION_FATAL(message_)
#endif /// !defined(LOG_FATAL)

#if !defined(IF_ERR_LOGGED_DEBUG_TRACE)
#define IF_ERR_LOGGED_DEBUG_TRACE(isLogged_, isErrLogged_, message_) \
    if (isLogged_) { IS_LOGGED_DEBUG(message_); } \
    else { if (isErrLogged_) { IS_ERR_LOGGED_DEBUG(message_); } \
           else { IS_ERR_LOGGED_TRACE(message_); } }
#endif /// !defined(IF_ERR_LOGGED_DEBUG_TRACE)

#endif /// _XOS_LOGGER_INTERFACE_HXX_
