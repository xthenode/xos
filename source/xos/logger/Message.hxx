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
///   File: Message.hxx
///
/// Author: $author$
///   Date: 5/6/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_LOGGER_MESSAGE_HXX_
#define _XOS_LOGGER_MESSAGE_HXX_

#include "xos/base/String.hxx"

namespace xos {
namespace logger {

typedef String::Implements FunctionTImplements;
typedef String FunctionTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: FunctionT
///////////////////////////////////////////////////////////////////////
template <class TImplements = FunctionTImplements, class TExtends = FunctionTExtends>
class _EXPORT_CLASS FunctionT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef Extends string_t;
    typedef typename Extends::char_t char_t;

    FunctionT(const string_t& name): Extends(name) {
    }
    FunctionT(const char_t* name): Extends(name) {
    }
    FunctionT(const FunctionT &copy): Extends(copy) {
    }
};
typedef FunctionT<> Function;

typedef String::Implements LocationTImplements;
typedef String LocationTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: LocationT
///////////////////////////////////////////////////////////////////////
template <class TImplements = LocationTImplements, class TExtends = LocationTExtends>
class _EXPORT_CLASS LocationT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef Extends string_t;
    typedef typename Extends::char_t char_t;
    typedef typename Extends::from_unsigned_t from_unsigned_t;

    LocationT
    (const string_t& functionName, const string_t& fileName, size_t lineNumber)
    : _functionName(functionName),
      _fileName(fileName), _lineNumber(lineNumber) {
    }
    LocationT
    (const char_t* functionName, const char_t* fileName, size_t lineNumber)
    : _functionName(functionName),
      _fileName(fileName), _lineNumber(lineNumber) {
    }
    LocationT(const LocationT &copy)
    : _functionName(copy._functionName),
      _fileName(copy._fileName), _lineNumber(copy._lineNumber) {
    }

    inline string_t FunctionName() const { return _functionName; }
    inline string_t FileName() const { return _fileName; }
    inline string_t LineNumber() const {
        from_unsigned_t from(_lineNumber);
        string_t s(from);
        return s;
    }

protected:
    string_t _functionName;
    string_t _fileName;
    size_t _lineNumber;
};
typedef LocationT<> Location;

typedef String::Implements MessageTImplements;
typedef String MessageTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: MessageT
///////////////////////////////////////////////////////////////////////
template <class TImplements = MessageTImplements, class TExtends = MessageTExtends>
class _EXPORT_CLASS MessageT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef Extends string_t;
    typedef typename Extends::char_t char_t;
    typedef typename Extends::from_signed_t from_signed_t;

    MessageT(const string_t& str): Extends(str) {
    }
    MessageT(const char_t* chars, size_t length): Extends(chars, length) {
    }
    MessageT(const char_t* chars): Extends(chars) {
    }
    MessageT(const MessageT &copy): Extends(copy) {
    }
    MessageT() {
    }

    MessageT& operator << (const string_t& str) { 
        this->Append(str.c_str()); 
        return *this; 
    }
    MessageT& operator << (const char_t* chars) { 
        this->Append(chars); 
        return *this; 
    }
    MessageT& operator << (int i) {
        from_signed_t from(i);
        string_t s(from);
        this->Append(s);
        return *this;
    }
};
typedef MessageT<> Message;

} /// namespace logger
} /// namespace xos

#endif /// _XOS_LOGGER_MESSAGE_HXX_
