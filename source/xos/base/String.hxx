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
#ifndef _XOS_BASE_STRING_HXX_
#define _XOS_BASE_STRING_HXX_

#include "xos/base/std/String.hxx"

namespace xos {

namespace based {
///////////////////////////////////////////////////////////////////////
///  Class: StringT
///////////////////////////////////////////////////////////////////////
template 
<typename TChar = char,
 typename TEnd = TChar, TEnd VEnd = 0, 
 class TChars = CharsT<TChar, TEnd, VEnd, ImplementBase>,
 class TString = Base, size_t VDefaultSize = 128,
 class TImplements = TChars, class TExtends = TString>

class _EXPORTED_ StringT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef TChars chars_t;
    typedef TChar char_t;
    typedef TEnd end_t;
    static const end_t endof = VEnd;
    static const size_t defaultSize = VDefaultSize;
    typedef typename TChars::from_signed_t from_signed_t;
    typedef typename TChars::from_unsigned_t from_unsigned_t;

    StringT(const char_t* chars, size_t length) {
        _construct();
        append(chars, length);
    }
    StringT(const char_t* chars) {
        _construct();
        append(chars);
    }
    StringT(const StringT &copy) {
        _construct();
        append(copy.c_str(), copy.length());
    }
    StringT() {
        _construct();
    }
    virtual ~StringT() {
        _destruct();
    }

    StringT& assign(const StringT& copy) {
        clear();
        return append(copy.c_str(), copy.length());
    }
    StringT& assign(const char_t* chars) {
        clear();
        return append(chars);
    }
    StringT& assign(const char_t* chars, size_t length) {
        clear();
        return append(chars, length);
    }

    StringT& append(const StringT& copy) {
        return append(copy.c_str(), copy.length());
    }
    StringT& append(const char_t* chars) {
        return append(chars, _length_of(chars));
    }
    StringT& append(const char_t* chars, size_t length) {
        if ((chars) && (0 < length)) {
            size_t newSize = (m_tell+length);
            if (m_size <= newSize) {
                if (m_fixedSize)
                    length = m_size-m_tell;
                else
                if (!(_adjust_to_size(_new_size_of(newSize))))
                    length = 0;
            }
            if (length) {
                _copy_to(m_writeBuffer+m_tell, chars, length);
                if ((m_tell += length) > m_length)
                if ((m_length = m_tell) < m_size)
                    m_writeBuffer[m_tell] = ((char_t)endof);
            }
        }
        return *this;
    }
    StringT& clear() {
        if ((m_writeBuffer)) {
            if (m_size > (m_length = (m_tell = 0))) {
                m_writeBuffer[m_tell] = ((char_t)endof);
            }
        }
        return *this;
    }

    virtual int compare(const StringT& to) {
        return compare(to.c_str(), to.length());
    }
    virtual int compare(const char_t* toChars) {
        return compare(toChars, _length_of(toChars));
    }
    virtual int compare(const char_t* toChars, size_t toLength) {
        int unequal = 2;
        const char_t* thisChars = 0;
        char_t toChar = 0, thisChar = 0;
        size_t thisLength = 0;
        ssize_t lendiff = 0;

        if (!(thisChars = (c_str()))) {
            return (toChars)?(-1):(1);
        }

        if (0 < (lendiff = (toLength - (thisLength = length())))) {
            toLength = thisLength;
            unequal = -1;
        } else {
            if (lendiff) {
                unequal = 1;
            }
        }

        if (toLength > 0) {
            do {
                if ((thisChar = *(thisChars++)) > (toChar = *(toChars++))) {
                    return 1;
                } else {
                    if (thisChar < toChar) {
                        return -1;
                    }
                }
            } while (--toLength > 0);
        }
        return unequal;
    }

    virtual char_t* buffer() const {
        return str();
    }
    virtual const char_t* chars() const {
        return c_str();
    }
    virtual char_t* str() const {
        return m_writeBuffer;
    }
    virtual const char_t* c_str() const {
        return m_readBuffer;
    }
    virtual size_t length() const {
        return m_length;
    }
    virtual size_t size() const {
        return m_size;
    }

protected:
    virtual void _construct() {
        m_freeBuffer = m_fixedSize = false;
        m_size = defaultSize;
        m_length = m_tell = 0;
        m_readBuffer = m_writeBuffer = m_defaultBuffer;
    }
    virtual void _destruct() {
        if ((m_writeBuffer != m_defaultBuffer)) {
            if ((m_writeBuffer) && (m_freeBuffer)) {
                _free_buffer(m_writeBuffer);
            }
        }
    }

    virtual size_t _adjust_size(size_t newSize) {
        if (m_writeBuffer) {
            if (m_size < (newSize)) {
                if (!m_fixedSize) {
                    if (0 < (_adjust_to_size(_new_size_of(newSize)))) {
                        return m_size;
                    }
                }
            } else {
                return m_size;
            }
        }
        return 0;
    }
    virtual size_t _adjust_to_size(size_t size) {
        if (m_writeBuffer) {
            if (size > m_size) {
                bool freeBuffer = false;
                char_t* buffer = 0;

                if (!(freeBuffer = m_freeBuffer)) {
                    m_freeBuffer = true;
                }
                if ((buffer = _alloc_buffer(size))) {
                    if (m_length > 0) {
                        _copy_to(buffer, m_writeBuffer, m_length);
                    }
                    if ((m_writeBuffer != m_defaultBuffer) && (freeBuffer)) {
                        _free_buffer(m_writeBuffer);
                    }
                    m_readBuffer = m_writeBuffer = buffer;
                    m_size = size;
                    return m_size;
                } else {
                    m_freeBuffer = freeBuffer;
                }
            } else {
                return m_size;
            }
        }
        return 0;
    }
    virtual size_t _new_size_of(size_t size) const {
        ///
        /// Increase the buffer size by default buffer
        /// sized chunks. Note the desired new size is always
        /// needed size + 1. The size in chunks is calculated
        /// as (new size + (chunk size - 1)) / chunk size.
        /// since new size = needed size + 1 then
        /// chunks = (needed size + chunk size) / chunk size.
        /// Finally bytes which is chunks * chunk size
        /// which can be reduced to
        /// ((needed size / chunk size) + 1) * chunk size
        ///
        return ((size / defaultSize) + 1) * defaultSize;
    }

    virtual size_t _copy_to
    (char_t* to, const char_t* from, size_t size) const {
        size_t length = 0;

        if ((to) && (from)) {
            for (length = size; size; --size) {
                (*to++) = (*from++);
            }
        }
        return length;
    }
    virtual size_t _length_of(const char_t* chars) const {
        const char_t endChar(((char_t)endof));
        size_t length = 0;

        if ((chars)) {
            for (length = 0; ((*chars) != endChar); chars++) {
                length++;
            }
        }
        return length;
    }

    virtual char_t* _alloc_buffer(size_t size) const {
        char_t* chars = new char_t[size];
        return chars;
    }
    virtual void _free_buffer(char_t* buffer) const {
        delete[] buffer;
    }

protected:
    bool m_freeBuffer, m_fixedSize;
    size_t m_size, m_length, m_tell;
    const char_t* m_readBuffer;
    char_t* m_writeBuffer;
    char_t m_defaultBuffer[defaultSize];
};
typedef StringT<char, char, 0> String;
typedef StringT<tchar_t, tchar_t, 0> TString;
typedef StringT<wchar_t, wchar_t, 0> WString;
typedef StringT<byte_t, byte_t, 0> ByteString;
typedef StringT<word_t, word_t, 0> WordString;
} /// namespace based

namespace base {
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
} /// namespace base

#if defined(USE_XOS_BASE_STRING)
namespace stringns = base;
#else // defined(USE_XOS_BASE_STRING)
namespace stringns = std;
#endif // defined(USE_XOS_BASE_STRING)

typedef stringns::String String;
typedef stringns::TString TString;
typedef stringns::WString WString;
typedef stringns::ByteString ByteString;
typedef stringns::WordString WordString;

///////////////////////////////////////////////////////////////////////
///  Class: StringT
///////////////////////////////////////////////////////////////////////
template 
<typename TChar = char,
 typename TEnd = TChar, TEnd VEnd = 0,
 class TChars = CharsT<TChar, TEnd, VEnd, ImplementBase>,
 class TString = stringns::based::StringT<TChar, TEnd, VEnd, TChars>,
 class TExtend = derived::StringT<TChar, TEnd, VEnd, TChars, TString>,
 class TImplements = TChars, class TExtends = TExtend>

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

///////////////////////////////////////////////////////////////////////
///  Class: UnsignedToStringT
///////////////////////////////////////////////////////////////////////
template 
<class TString = String,
 class TImplements = typename TString::Implements, class TExtends = TString>
class _EXPORTED_ UnsignedToStringT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    UnsignedToStringT(const unsigned& value) {
        this->AppendUnsigned(value);
    }
    UnsignedToStringT(const UnsignedToStringT& copy): Extends(copy) {
    }
    virtual ~UnsignedToStringT() {
    }
}; /// class _EXPORTED_ UnsignedToStringT
typedef UnsignedToStringT<String> UnsignedToString;
typedef UnsignedToStringT<TString> UnsignedToTString;
typedef UnsignedToStringT<WString> UnsignedToWString;

///////////////////////////////////////////////////////////////////////
///  Class: IntToStringT
///////////////////////////////////////////////////////////////////////
template 
<class TString = String,
 class TImplements = typename TString::Implements, class TExtends = TString>
class _EXPORTED_ IntToStringT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    IntToStringT(const unsigned& value) {
        this->AppendInt(value);
    }
    IntToStringT(const IntToStringT& copy): Extends(copy) {
    }
    virtual ~IntToStringT() {
    }
}; /// class _EXPORTED_ IntToStringT
typedef IntToStringT<String> IntToString;
typedef IntToStringT<TString> IntToTString;
typedef IntToStringT<WString> IntToWString;

} /// namespace xos

#endif /// _XOS_BASE_STRING_HXX_
