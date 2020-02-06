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
///   File: String.hxx
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_BASE_STD_STRING_HXX_
#define _XOS_BASE_STD_STRING_HXX_

#include "xos/base/Chars.hxx"
#include <sstream>
#include <string>

namespace xos {
namespace std {
namespace based {
///////////////////////////////////////////////////////////////////////
///  Class: StringT
///////////////////////////////////////////////////////////////////////
template 
<typename TChar = char,
 typename TEnd = TChar, TEnd VEnd = 0, 
 class TChars = CharsT<TChar, TEnd, VEnd, ImplementBase>,
 class TString = ::std::basic_string<TChar>,
 class TSStream = ::std::basic_stringstream<TChar>,
 class TImplements = TChars, class TExtends = TString>

class _EXPORTED_ StringT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef TSStream sstream_t;
    typedef TString string_t;
    typedef TChars chars_t;
    typedef TChar char_t;
    typedef TEnd end_t;
    static const end_t endof = VEnd;
    typedef typename TChars::from_signed_t from_signed_t;
    typedef typename TChars::from_unsigned_t from_unsigned_t;

    StringT(const from_signed_t& from) {
        this->append_signed(from);
    }
    StringT(const from_unsigned_t& from) {
        this->append_unsigned(from);
    }
    StringT(const char_t* chars, size_t length) {
        if ((chars) && (length)) {
            this->append(chars, length);
        }
    }
    StringT(const char_t* chars) {
        if ((chars)) {
            this->append(chars);
        }
    }
    StringT(const StringT &copy) {
        this->append(copy);
    }
    StringT() {
    }
    virtual ~StringT() {
    }

    virtual StringT& assign_signed(signed from) {
        this->clear();
        this->append_signed(from);
        return *this;
    }
    virtual StringT& assign_unsigned(unsigned from) {
        this->clear();
        this->append_unsigned(from);
        return *this;
    }
    virtual StringT& append_signed(signed from) {
        sstream_t ss; ss << from;
        this->append(ss.str());
        return *this;
    }
    virtual StringT& append_unsigned(unsigned from) {
        sstream_t ss; ss << from;
        this->append(ss.str());
        return *this;
    }
    virtual signed to_signed() const {
        sstream_t ss(*this);
        signed to = 0;
        ss >> to;
        return to;
    }
    virtual unsigned to_unsigned() const {
        sstream_t ss(*this);
        unsigned to = 0;
        ss >> to;
        return to;
    }

    virtual char_t* buffer() const {
        return this->str();
    }
    virtual const char_t* chars() const {
        return this->c_str();
    }
    virtual char_t* str() const {
        return 0;
    }
    virtual size_t size() const {
        return 0;
    }
};
typedef StringT<char, char, 0> String;
typedef StringT<tchar_t, tchar_t, 0> TString;
typedef StringT<wchar_t, wchar_t, 0> WString;
typedef StringT<byte_t, byte_t, 0> ByteString;
typedef StringT<word_t, word_t, 0> WordString;
} /// namespace based
} /// namespace std
} /// namespace xos

namespace xos {
namespace derived {
///////////////////////////////////////////////////////////////////////
///  Class: StringT
///////////////////////////////////////////////////////////////////////
template 
<typename TChar = char,
 typename TEnd = TChar, TEnd VEnd = 0,
 class TChars = CharsT<TChar, TEnd, VEnd, ImplementBase>,
 class TString = std::based::StringT<TChar, TEnd, VEnd, TChars>,
 class TImplements = TChars, class TExtends = TString>

class _EXPORTED_ StringT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef TChars chars_t;
    typedef TChar char_t;
    typedef TEnd end_t;
    static const end_t endof = VEnd;
    typedef typename TString::from_signed_t from_signed_t;
    typedef typename TString::from_unsigned_t from_unsigned_t;

    StringT(const from_signed_t& from) {
        this->AppendSigned(from);
    }
    StringT(const from_unsigned_t& from) {
        this->AppendUnsigned(from);
    }
    StringT(const char_t* chars, size_t length): Extends(chars, length) {
    }
    StringT(const char_t* chars): Extends(chars) {
    }
    StringT(const StringT &copy): Extends(copy) {
    }
    StringT() {
    }
    virtual ~StringT() {
    }

    virtual StringT& AssignL(const char_t* chars, ...) {
        va_list va; va_start(va, chars);
        this->AssignV(chars, va);
        va_end(va);
        return *this;
    }
    virtual StringT& AssignV(const char_t* chars, va_list va) {
        this->Clear();
        this->AppendV(chars, va);
        return *this;
    }
    virtual StringT& AppendL(const char_t* chars, ...) {
        va_list va; va_start(va, chars);
        this->AppendV(chars, va);
        va_end(va);
        return *this;
    }
    virtual StringT& AppendV(const char_t* chars, va_list va) {
        typedef const char_t* chars_t;
        if ((chars)) {
            do { this->Append(chars);
            } while ((chars = va_arg(va, chars_t)));
        }
        return *this;
    }

    virtual StringT& Assign(const Extends& copy) {
        this->Clear();
        this->Append(copy);
        return *this;
    }
    virtual StringT& Assign(const char_t* chars) {
        this->Clear();
        this->Append(chars);
        return *this;
    }
    virtual StringT& Assign(const char_t* chars, size_t length) {
        this->Clear();
        this->Append(chars, length);
        return *this;
    }
    virtual StringT& Append(const Extends& copy) {
        this->append(copy);
        return *this;
    }
    virtual StringT& Append(const char_t* chars) {
        this->append(chars);
        return *this;
    }
    virtual StringT& Append(const char_t* chars, size_t length) {
        this->append(chars, length);
        return *this;
    }
    virtual StringT& Clear() {
        this->clear();
        return *this;
    }

    virtual StringT& AssignSigned(signed from) {
        this->clear();
        this->AppendSigned(from);
        return *this;
    }
    virtual StringT& AssignUnsigned(unsigned from) {
        this->clear();
        this->AppendUnsigned(from);
        return *this;
    }
    virtual StringT& AppendSigned(signed from) {
        TChar c = ((TChar)('0'));
        TChar s = ((TChar)('-'));
        size_t digits = 0;
        signed msdValue = 0;
    
        if (0 != from) {
            if (0 > from) {
                this->append(&s, 1);
                from = -from;
            }
            for (digits = 0, msdValue = 0;
                 from; from /= 10, digits++) {
                msdValue = (msdValue*10) + (from%10);
            }
            for (; msdValue; msdValue /= 10, --digits) {
                TChar d = c + (TChar)(msdValue%10);
                this->append(&d, 1);
            }
            for (; digits; --digits)
                this->append(&c, 1);
        } else {
            this->append(&c, 1);
        }
        return *this;
    }
    virtual StringT& AppendUnsigned(unsigned from) {
        TChar c = ((TChar)('0'));
        size_t digits = 0;
        unsigned msdValue = 0;
    
        if (0 != from) {
            for (digits = 0, msdValue = 0;
                 from; from /= 10, digits++) {
                msdValue = (msdValue*10) + (from%10);
            }
            for (; msdValue; msdValue /= 10, --digits) {
                TChar d = c + (TChar)(msdValue%10);
                this->append(&d, 1);
            }
            for (; digits; --digits)
                this->append(&c, 1);
        } else {
            this->append(&c, 1);
        }
        return *this;
    }
    virtual signed ToSigned() const {
        signed to = 0;
        to = Implements::ToSigned(this->chars(), this->length());
        return to;
    }
    virtual unsigned ToUnsigned() const {
        unsigned to = 0;
        to = Implements::ToUnsigned(this->chars(), this->length());
        return to;
    }
    
    virtual const char_t* HasChars(size_t& length) const {
        size_t len = 0;
        if ((len = this->Length())) {
            const char_t* chars = 0;
            if ((chars = this->Chars())) {
                length = len;
                return chars;
            }
        }
        return 0;
    }
    virtual const char_t* HasChars() const {
        if ((this->Length())) {
            return this->Chars();
        }
        return 0;
    }
    virtual const char_t* Chars(size_t& length) const {
        const char_t* chars = 0;
        if ((chars = this->Chars())) {
            length = this->Length();
        }
        return chars;
    }
    virtual const char_t* Chars() const {
        return this->c_str();
    }
    virtual size_t Length() const {
        return this->length();
    }
};
} /// namespace derived
} /// namespace xos

namespace xos {
namespace std {
namespace derived {
///////////////////////////////////////////////////////////////////////
///  Class: StringT
///////////////////////////////////////////////////////////////////////
template 
<typename TChar = char,
 typename TEnd = TChar, TEnd VEnd = 0,
 class TChars = CharsT<TChar, TEnd, VEnd, ImplementBase>,
 class TString = xos::derived::StringT<TChar, TEnd, VEnd, TChars>,
 class TImplements = TChars, class TExtends = TString>

class _EXPORTED_ StringT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef TChars chars_t;
    typedef TChar char_t;
    typedef TEnd end_t;
    static const end_t endof = VEnd;
    typedef typename TString::from_signed_t from_signed_t;
    typedef typename TString::from_unsigned_t from_unsigned_t;

    StringT(const from_signed_t& from) {
        this->AppendSigned(from);
    }
    StringT(const from_unsigned_t& from) {
        this->AppendUnsigned(from);
    }
    StringT(const char_t* chars, size_t length): Extends(chars, length) {
    }
    StringT(const char_t* chars): Extends(chars) {
    }
    StringT(const StringT &copy): Extends(copy) {
    }
    StringT() {
    }
    virtual ~StringT() {
    }
};
} /// namespace derived

typedef xos::derived::StringT
<char, char, 0, based::String::chars_t, based::String> String;

typedef xos::derived::StringT
<tchar_t, tchar_t, 0, based::TString::chars_t, based::TString> TString;

typedef xos::derived::StringT
<wchar_t, wchar_t, 0, based::WString::chars_t, based::WString> WString;

typedef xos::derived::StringT
<byte_t, byte_t, 0, based::ByteString::chars_t, based::ByteString> ByteString;

typedef xos::derived::StringT
<word_t, word_t, 0, based::WordString::chars_t, based::WordString> WordString;
} /// namespace std
} /// namespace xos

#endif /// _XOS_BASE_STD_STRING_HXX_
