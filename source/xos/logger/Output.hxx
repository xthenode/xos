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
///   File: Output.hxx
///
/// Author: $author$
///   Date: 5/6/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_LOGGER_OUTPUT_HXX_
#define _XOS_LOGGER_OUTPUT_HXX_

#include "xos/logger/Enabled.hxx"
#include "xos/logger/Message.hxx"
#include <iostream>

namespace xos {
namespace logger {

typedef Enabled OutputTImplements;
///////////////////////////////////////////////////////////////////////
///  Class: OutputT
///////////////////////////////////////////////////////////////////////
template <class TImplements = OutputTImplements>
class _EXPORT_CLASS OutputT: virtual public TImplements {
public:
    typedef TImplements Implements;
    typedef Location::string_t string_t;
    typedef Location::char_t char_t;
    typedef const char_t* const_chars_t;

protected:
    virtual void Out(const Location& _location) {
        string_t beforeFileName(""), afterFileName(""),
                 beforeLineNumber("["), afterLineNumber("]"),
                 beforeFunctionName(" "), afterFunctionName(" ");
        OutL
        (beforeFileName.c_str(), _location.FileName().c_str(), afterFileName.c_str(),
         beforeLineNumber.c_str(), _location.LineNumber().c_str(), afterLineNumber.c_str(),
         beforeFunctionName.c_str(), _location.FunctionName().c_str(), afterFunctionName.c_str(), NULL);
    }
    virtual void Out(const Function& _function) {
        Out(_function.c_str());
        Out(": ");
    }
    virtual void Out(const Message& _message) {
        Out(_message.c_str());
    }
    virtual void OutL(const char_t* chars, ...) {
        va_list va;
        va_start(va, chars);
        OutV(chars, va);
        va_end(va);
    }
    virtual void OutV(const char_t* chars, va_list va) {
        while (chars) {
            Out(chars);
            chars = va_arg(va, const_chars_t);
        }
    }
    virtual void Out(const char_t* chars, size_t length) {
        if ((chars) && (length)) {
            for (char_t c = *(chars); length; --length, c = *(++chars)) {
                Out(c);
            }
        }
    }
    virtual void Out(const char_t* chars) {
        if ((chars)) {
            for (char_t c = *(chars); c; c = *(++chars)) {
                Out(c);
            }
        }
    }
    virtual void OutLn() {
        const char_t c = ((char_t)'\n');
        Out(c);
        OutFlush();
    }
    virtual void Out(const char_t& c) {
    }
    virtual void OutFlush() {
    }
};
typedef OutputT<> Output;

} /// namespace logger
} /// namespace xos

///
///
/// 
#if defined(_MSC_VER)
#define __XOS_LOGGER_FUNCTION__ __FUNCTION__
#else /// defined(_MSC_VER)
#endif /// defined(_MSC_VER)

#if !defined(__XOS_LOGGER_FUNCTION__)
#define __XOS_LOGGER_FUNCTION__ __FUNCTION__
#endif /// !defined(__XOS_LOGGER_FUNCTION__)

#if !defined(XOS_LOGGER_FUNCTION)
#define XOS_LOGGER_FUNCTION \
    ::xos::logger::Function(__XOS_LOGGER_FUNCTION__)
#endif /// !defined(XOS_LOGGER_LOCATION)

#if !defined(XOS_LOGGER_LOCATION)
#define XOS_LOGGER_LOCATION \
    ::xos::logger::Location(__XOS_LOGGER_FUNCTION__, __FILE__, __LINE__)
#endif /// !defined(XOS_LOGGER_LOCATION)

///
///
/// 
#define XOS_COSTREAM_LOG_PLAIN(ostream_, message_) \
{ ::xos::logger::Message message; \
  ostream_ << message << message_ << "\n"; }

#define XOS_COSTREAM_LOG_FUNCTION(ostream_, message_) \
{ ::xos::logger::Message message; \
  ostream_ << XOS_LOGGER_FUNCTION << ": " << message << message_ << "\n"; }

#define XOS_COSTREAM_LOG_LOCATION(ostream_, message_) \
{ ::xos::logger::Location location = XOS_LOGGER_LOCATION; \
  ::xos::logger::Message message; \
  ostream_ <<  location.FileName().chars() << "[" <<  location.LineNumber().chars() << "] " \
  <<  location.FunctionName().chars() << ": " << message << message_ << "\n"; }

///
///
/// 
#define XOS_STDSTREAM_LOG_PLAIN(stream_, message_) \
{ ::xos::logger::Message message; message << message_; \
  fprintf(stream_, "%s\n", message.chars()); }

#define XOS_STDSTREAM_LOG_FUNCTION(stream_, message_) \
{ ::xos::logger::Function function = XOS_LOGGER_FUNCTION; \
  ::xos::logger::Message message; message << message_; \
  fprintf(stream_, "%s: %s\n", function.chars(), message.chars()); }

#define XOS_STDSTREAM_LOG_LOCATION(stream_, message_) \
{ ::xos::logger::Location location = XOS_LOGGER_LOCATION; \
  ::xos::logger::Message message; message << message_; \
  fprintf(stream_, "%s[%s] %s: %s\n", location.FileName().chars(), \
  location.LineNumber().chars(), location.FunctionName().chars(), message.chars()); }

///
///
/// 
#define XOS_CERR_LOG_PLAIN(message_) XOS_COSTREAM_LOG_PLAIN(::std::cerr, message_)
#define XOS_STDERR_LOG_PLAIN(message_) XOS_STDSTREAM_LOG_PLAIN(stderr, message_)
#define XOS_CERR_LOG_FUNCTION(message_) XOS_COSTREAM_LOG_FUNCTION(::std::cerr, message_)
#define XOS_STDERR_LOG_FUNCTION(message_) XOS_STDSTREAM_LOG_FUNCTION(stderr, message_)
#define XOS_CERR_LOG_LOCATION(message_) XOS_COSTREAM_LOG_LOCATION(::std::cerr, message_)
#define XOS_STDERR_LOG_LOCATION(message_) XOS_STDSTREAM_LOG_LOCATION(stderr, message_)

#if defined(XOS_PLAIN_LOGGING)
#define XOS_CERR_LOG XOS_CERR_LOG_PLAIN
#define XOS_STDERR_LOG XOS_STDERR_LOG_PLAIN
#else /// defined(XOS_PLAIN_LOGGING)
#if defined(XOS_FUNCTION_LOGGING)
#define XOS_CERR_LOG XOS_CERR_LOG_FUNCTION
#define XOS_STDERR_LOG XOS_STDERR_LOG_FUNCTION
#else /// defined(XOS_FUNCTION_LOGGING)
#define XOS_CERR_LOG XOS_CERR_LOG_LOCATION
#define XOS_STDERR_LOG XOS_STDERR_LOG_LOCATION
#endif /// defined(XOS_FUNCTION_LOGGING)
#endif /// defined(XOS_PLAIN_LOGGING)

///
///
/// 
#if defined(TRACE_BUILD)
#define XOS_CERR_LOG_TRACE(message_) XOS_CERR_LOG(message_)
#define XOS_STDERR_LOG_TRACE(message_) XOS_STDERR_LOG(message_)
#define XOS_CERR_LOG_PLAIN_TRACE(message_) XOS_CERR_LOG_PLAIN(message_)
#define XOS_STDERR_LOG_PLAIN_TRACE(message_) XOS_STDERR_LOG_PLAIN(message_)
#define XOS_CERR_LOG_FUNCTION_TRACE(message_) XOS_CERR_LOG_FUNCTION(message_)
#define XOS_STDERR_LOG_FUNCTION_TRACE(message_) XOS_STDERR_LOG_FUNCTION(message_)
#define XOS_CERR_LOG_LOCATION_TRACE(message_) XOS_CERR_LOG_LOCATION(message_)
#define XOS_STDERR_LOG_LOCATION_TRACE(message_) XOS_STDERR_LOG_LOCATION(message_)
#else /// defined(TRACE_BUILD)
#define XOS_CERR_LOG_TRACE(message_)
#define XOS_STDERR_LOG_TRACE(message_)
#define XOS_CERR_LOG_PLAIN_TRACE(message_)
#define XOS_STDERR_LOG_PLAIN_TRACE(message_)
#define XOS_CERR_LOG_FUNCTION_TRACE(message_)
#define XOS_STDERR_LOG_FUNCTION_TRACE(message_)
#define XOS_CERR_LOG_LOCATION_TRACE(message_)
#define XOS_STDERR_LOG_LOCATION_TRACE(message_)
#endif /// defined(TRACE_BUILD)

#if defined(DEBUG_BUILD)
#define XOS_CERR_LOG_DEBUG(message_) XOS_CERR_LOG(message_)
#define XOS_STDERR_LOG_DEBUG(message_) XOS_STDERR_LOG(message_)
#define XOS_CERR_LOG_PLAIN_DEBUG(message_) XOS_CERR_LOG_PLAIN(message_)
#define XOS_STDERR_LOG_PLAIN_DEBUG(message_) XOS_STDERR_LOG_PLAIN(message_)
#define XOS_CERR_LOG_FUNCTION_DEBUG(message_) XOS_CERR_LOG_FUNCTION(message_)
#define XOS_STDERR_LOG_FUNCTION_DEBUG(message_) XOS_STDERR_LOG_FUNCTION(message_)
#define XOS_CERR_LOG_LOCATION_DEBUG(message_) XOS_CERR_LOG_LOCATION(message_)
#define XOS_STDERR_LOG_LOCATION_DEBUG(message_) XOS_STDERR_LOG_LOCATION(message_)
#else /// defined(DEBUG_BUILD)
#define XOS_CERR_LOG_DEBUG(message_)
#define XOS_STDERR_LOG_DEBUG(message_)
#define XOS_CERR_LOG_FUNCTION_DEBUG(message_)
#define XOS_STDERR_LOG_FUNCTION_DEBUG(message_)
#define XOS_CERR_LOG_LOCATION_DEBUG(message_)
#define XOS_STDERR_LOG_LOCATION_DEBUG(message_)
#endif /// defined(DEBUG_BUILD)

#define XOS_CERR_LOG_ERROR(message_) XOS_CERR_LOG(message_)
#define XOS_STDERR_LOG_ERROR(message_) XOS_STDERR_LOG(message_)
#define XOS_CERR_LOG_PLAIN_ERROR(message_) XOS_CERR_LOG_PLAIN(message_)
#define XOS_STDERR_LOG_PLAIN_ERROR(message_) XOS_STDERR_LOG_PLAIN(message_)
#define XOS_CERR_LOG_FUNCTION_ERROR(message_) XOS_CERR_LOG_FUNCTION(message_)
#define XOS_STDERR_LOG_FUNCTION_ERROR(message_) XOS_STDERR_LOG_FUNCTION(message_)
#define XOS_CERR_LOG_LOCATION_ERROR(message_) XOS_CERR_LOG_LOCATION(message_)
#define XOS_STDERR_LOG_LOCATION_ERROR(message_) XOS_STDERR_LOG_LOCATION(message_)

///
/// XOS_ERR_LOG_ 
/// 
#if defined(XOS_CERR_LOGGING)
#define XOS_ERR_LOG_TRACE(message_) XOS_CERR_LOG_TRACE(message_)
#define XOS_ERR_LOG_PLAIN_TRACE(message_) XOS_CERR_LOG_PLAIN_TRACE(message_)
#define XOS_ERR_LOG_FUNCTION_TRACE(message_) XOS_CERR_LOG_FUNCTION_TRACE(message_)
#define XOS_ERR_LOG_LOCATION_TRACE(message_) XOS_CERR_LOG_LOCATION_TRACE(message_)
#define XOS_ERR_LOG_DEBUG(message_) XOS_CERR_LOG_DEBUG(message_)
#define XOS_ERR_LOG_PLAIN_DEBUG(message_) XOS_CERR_LOG_PLAIN_DEBUG(message_)
#define XOS_ERR_LOG_FUNCTION_DEBUG(message_) XOS_CERR_LOG_FUNCTION_DEBUG(message_)
#define XOS_ERR_LOG_LOCATION_DEBUG(message_) XOS_CERR_LOG_LOCATION_DEBUG(message_)
#define XOS_ERR_LOG_ERROR(message_) XOS_CERR_LOG_ERROR(message_)
#define XOS_ERR_LOG_PLAIN_ERROR(message_) XOS_CERR_LOG_PLAIN_ERROR(message_)
#define XOS_ERR_LOG_FUNCTION_ERROR(message_) XOS_CERR_LOG_FUNCTION_ERROR(message_)
#define XOS_ERR_LOG_LOCATION_ERROR(message_) XOS_CERR_LOG_LOCATION_ERROR(message_)
#else /// defined(XOS_CERR_LOGGING)
#define XOS_ERR_LOG_TRACE(message_) XOS_STDERR_LOG_TRACE(message_)
#define XOS_ERR_LOG_PLAIN_TRACE(message_) XOS_STDERR_LOG_PLAIN_TRACE(message_)
#define XOS_ERR_LOG_FUNCTION_TRACE(message_) XOS_STDERR_LOG_FUNCTION_TRACE(message_)
#define XOS_ERR_LOG_LOCATION_TRACE(message_) XOS_STDERR_LOG_LOCATION_TRACE(message_)
#define XOS_ERR_LOG_DEBUG(message_) XOS_STDERR_LOG_DEBUG(message_)
#define XOS_ERR_LOG_PLAIN_DEBUG(message_) XOS_STDERR_LOG_PLAIN_DEBUG(message_)
#define XOS_ERR_LOG_FUNCTION_DEBUG(message_) XOS_STDERR_LOG_FUNCTION_DEBUG(message_)
#define XOS_ERR_LOG_LOCATION_DEBUG(message_) XOS_STDERR_LOG_LOCATION_DEBUG(message_)
#define XOS_ERR_LOG_ERROR(message_) XOS_STDERR_LOG_ERROR(message_)
#define XOS_ERR_LOG_PLAIN_ERROR(message_) XOS_STDERR_LOG_PLAIN_ERROR(message_)
#define XOS_ERR_LOG_FUNCTION_ERROR(message_) XOS_STDERR_LOG_FUNCTION_ERROR(message_)
#define XOS_ERR_LOG_LOCATION_ERROR(message_) XOS_STDERR_LOG_LOCATION_ERROR(message_)
#endif /// defined(XOS_CERR_LOGGING)

#if !defined(ERR_LOG_TRACE)
#define ERR_LOG_TRACE(message_) XOS_ERR_LOG_TRACE(message_)
#define ERR_LOG_PLAIN_TRACE(message_) XOS_ERR_LOG_PLAIN_TRACE(message_)
#define ERR_LOG_FUNCTION_TRACE(message_) XOS_ERR_LOG_FUNCTION_TRACE(message_)
#define ERR_LOG_LOCATION_TRACE(message_) XOS_ERR_LOG_LOCATION_TRACE(message_)
#define ERR_LOG_DEBUG(message_) XOS_ERR_LOG_DEBUG(message_)
#define ERR_LOG_PLAIN_DEBUG(message_) XOS_ERR_LOG_PLAIN_DEBUG(message_)
#define ERR_LOG_FUNCTION_DEBUG(message_) XOS_ERR_LOG_FUNCTION_DEBUG(message_)
#define ERR_LOG_LOCATION_DEBUG(message_) XOS_ERR_LOG_LOCATION_DEBUG(message_)
#define ERR_LOG_ERROR(message_) XOS_ERR_LOG_ERROR(message_)
#define ERR_LOG_PLAIN_ERROR(message_) XOS_ERR_LOG_PLAIN_ERROR(message_)
#define ERR_LOG_FUNCTION_ERROR(message_) XOS_ERR_LOG_FUNCTION_ERROR(message_)
#define ERR_LOG_LOCATION_ERROR(message_) XOS_ERR_LOG_LOCATION_ERROR(message_)
#endif /// !defined(ERR_LOG_TRACE)

#endif /// _XOS_LOGGER_OUTPUT_HXX_
