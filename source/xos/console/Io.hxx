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
///   File: Io.hxx
///
/// Author: $author$
///   Date: 5/6/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_CONSOLE_IO_HXX_
#define _XOS_CONSOLE_IO_HXX_

#include "xos/base/Locked.hxx"
#include "xos/base/Chars.hxx"
#include "xos/base/String.hxx"

namespace xos {
namespace console {

typedef Locked IoTImplements;
///////////////////////////////////////////////////////////////////////
///  Class: IoT
///////////////////////////////////////////////////////////////////////
template 
<typename TChar = char, 
 typename TEndChar = TChar, TEndChar VEndChar = 0,
 class TChars = CharsT<TChar, TEndChar, VEndChar>,
 class TString = stringns::derived::StringT<TChar, TEndChar, VEndChar, TChars>,
 class TImplements = IoTImplements>

class _EXPORT_CLASS IoT: virtual public TImplements {
public:
    typedef TImplements Implements;
    
    typedef TString string_t;
    typedef TChars chars_t;
    typedef TChar char_t;
    typedef TEndChar endchar_t;
    static const TEndChar endchar = VEndChar;
    typedef const char_t* const_chars_t;

    virtual ssize_t ErrLn(const char_t* chars, size_t length) {
        ssize_t count = this->OutLn(this->OutStdErr(), chars, length);
        return count;
    }
    virtual ssize_t ErrLn(const char_t* chars) {
        ssize_t count = this->OutLn(this->OutStdErr(), chars);
        return count;
    }
    virtual ssize_t ErrLn() {
        ssize_t count = this->OutLn(this->OutStdErr());
        return count;
    }
    virtual ssize_t ErrF(const char_t* chars, ...) {
        ssize_t count = 0;
        va_list va;
        va_start(va, chars);
        count = ErrFV(chars, va);
        va_end(va);
        return count;
    }
    virtual ssize_t ErrFV(const char_t* chars, va_list va) {
        ssize_t count = this->OutFV(this->OutStdErr(), chars, va);
        return count;
    }
    virtual ssize_t ErrL(const char_t* chars, ...) {
        ssize_t count = 0;
        va_list va;
        va_start(va, chars);
        count = ErrV(chars, va);
        va_end(va);
        return count;
    }
    virtual ssize_t ErrLLn(const char_t* chars, ...) {
        ssize_t count = 0;
        va_list va;
        va_start(va, chars);
        if (0 <= (count = ErrV(chars, va))) {
            ssize_t amount = 0;
            if (0 < (amount = this->ErrLn())) {
                count += amount;
            } else {
                return amount;
            }
        }
        va_end(va);
        return count;
    }
    virtual ssize_t ErrV(const char_t* chars, va_list va) {
        ssize_t count = this->OutV(this->OutStdErr(), chars, va);
        return count;
    }
    virtual ssize_t Err(const char_t* chars, size_t length) {
        ssize_t count = this->Out(this->OutStdErr(), chars, length);
        return count;
    }
    virtual ssize_t Err(const char_t* chars) {
        ssize_t count = this->Out(this->OutStdErr(), chars);
        return count;
    }
    virtual ssize_t ErrFlush() {
        ssize_t count = this->OutFlush(this->OutStdErr());
        return count;
    }

    virtual ssize_t OutLn(const char_t* chars, size_t length) {
        ssize_t count = this->OutLn(this->OutStdOut(), chars, length);
        return count;
    }
    virtual ssize_t OutLn(const char_t* chars) {
        ssize_t count = this->OutLn(this->OutStdOut(), chars);
        return count;
    }
    virtual ssize_t OutLn() {
        ssize_t count = this->OutLn(this->OutStdOut());
        return count;
    }
    virtual ssize_t OutF(const char_t* chars, ...) {
        ssize_t count = 0;
        va_list va;
        va_start(va, chars);
        count = OutFV(chars, va);
        va_end(va);
        return count;
    }
    virtual ssize_t OutFV(const char_t* chars, va_list va) {
        ssize_t count = this->OutFV(this->OutStdOut(), chars, va);
        return count;
    }
    virtual ssize_t OutL(const char_t* chars, ...) {
        ssize_t count = 0;
        va_list va;
        va_start(va, chars);
        count = OutV(chars, va);
        va_end(va);
        return count;
    }
    virtual ssize_t OutLLn(const char_t* chars, ...) {
        ssize_t count = 0;
        va_list va;
        va_start(va, chars);
        if (0 <= (count = OutV(chars, va))) {
            ssize_t amount = 0;
            if (0 < (amount = this->OutLn())) {
                count += amount;
            } else {
                return amount;
            }
        }
        va_end(va);
        return count;
    }
    virtual ssize_t OutV(const char_t* chars, va_list va) {
        ssize_t count = this->OutV(this->OutStdOut(), chars, va);
        return count;
    }
    virtual ssize_t Out(const char_t* chars, size_t length) {
        ssize_t count = this->Out(this->OutStdOut(), chars, length);
        return count;
    }
    virtual ssize_t Out(const char_t* chars) {
        ssize_t count = this->Out(this->OutStdOut(), chars);
        return count;
    }
    virtual ssize_t OutFlush() {
        ssize_t count = this->OutFlush(this->OutStdOut());
        return count;
    }

    virtual ssize_t InLn(char_t* chars, size_t size) {
        ssize_t count = this->InLn(this->InStdIn(), chars, size);
        return count;
    }
    virtual ssize_t InLn() {
        ssize_t count = this->InLn(this->InStdIn());
        return count;
    }
    virtual ssize_t InF(char_t* chars, ...) {
        ssize_t count = 0;
        va_list va;
        va_start(va, chars);
        count = InFV(chars, va);
        va_end(va);
        return count;
    }
    virtual ssize_t InFV(char_t* chars, va_list va) {
        ssize_t count = this->InFV(this->InStdIn(), chars, va);
        return count;
    }
    virtual ssize_t In(char_t* chars, size_t size) {
        ssize_t count = this->In(this->InStdIn(), chars, size);
        return count;
    }

    virtual ssize_t OutF(FILE* out, const char_t* chars, ...) {
        ssize_t count = 0;
        va_list va;
        va_start(va, chars);
        count = OutFV(out, chars, va);
        va_end(va);
        return count;
    }
    virtual ssize_t OutFV(FILE* out, const char_t* chars, va_list va) {
        ssize_t count = 0;
        return count;
    }
    virtual ssize_t OutL(FILE* out, const char_t* chars, ...) {
        ssize_t count = 0;
        va_list va;
        va_start(va, chars);
        count = OutV(out, chars, va);
        va_end(va);
        return count;
    }
    virtual ssize_t OutLLn(FILE* out, const char_t* chars, ...) {
        ssize_t count = 0;
        va_list va;
        va_start(va, chars);
        if (0 <= (count = OutV(out, chars, va))) {
            ssize_t amount = 0;
            if (0 < (amount = this->OutLn(out))) {
                count += amount;
            } else {
                return amount;
            }
        }
        va_end(va);
        return count;
    }
    virtual ssize_t OutV(FILE* out, const char_t* chars, va_list va) {
        ssize_t count = 0, amount = 0;
        for (count = 0; chars; count += amount) {
            if (0 > (amount = this->Out(out, chars))) {
                return amount;
            }
            chars = va_arg(va, const_chars_t);
        }
        return count;
    }
    virtual ssize_t OutLn(FILE* out, const char_t* chars, size_t length) {
        ssize_t count = 0;
        if (0 <= (count = this->Out(out, chars, length))) {
            ssize_t amount = 0;
            if (0 < (amount = this->OutLn(out))) {
                count += amount;
            } else {
                return amount;
            }
        }
        return count;
    }
    virtual ssize_t OutLn(FILE* out, const char_t* chars) {
        ssize_t count = 0;
        if (0 <= (count = this->Out(out, chars))) {
            ssize_t amount = 0;
            if (0 < (amount = this->OutLn(out))) {
                count += amount;
            } else {
                return amount;
            }
        }
        return count;
    }
    virtual ssize_t OutLn(FILE* out) {
        char_t ln = ((char_t)'\n');
        ssize_t count = Out(out, &ln, 1);
        return count;
    }
    virtual ssize_t Out(FILE* out, const char_t* chars) {
        ssize_t count = 0;
        if ((out) && (chars) && (chars[0] != endchar)) {
            ssize_t amount = 0;
            char_t c = endchar;
            for (c = *(chars); c != endchar; c = *(++chars)) {
                if (0 < (amount = Out(out, &c, sizeof(char_t), 1))) {
                    count += amount;
                } else {
                    return amount;
                }
            }
        }
        return count;
    }
    virtual ssize_t Out(FILE* out, const char_t* chars, size_t length) {
        ssize_t count = 0;
        if ((out) && (chars) && (length)) {
            ssize_t amount = 0;
            if (0 < (amount = Out(out, chars, sizeof(char_t), length))) {
                count += amount;
            } else {
                return amount;
            }
        }
        return count;
    }
    virtual ssize_t Out(FILE* out, const char_t* chars, size_t size, size_t length) {
        ssize_t count = 0;
        if ((out) && (chars) && (length)) {
            ssize_t amount = 0;
            if (0 < (amount = ::fwrite(chars, size, length, out))) {
                count += amount;
            } else {
                return amount;
            }
        }
        return count;
    }
    virtual ssize_t OutFlush(FILE* out) {
        ssize_t count = 0;
        if ((out)) {
            int err = 0;
            err = ::fflush(out);
        }
        return count;
    }

    virtual ssize_t InF(FILE* in, char_t* chars, ...) {
        ssize_t count = 0;
        va_list va;
        va_start(va, chars);
        count = InFV(in, chars, va);
        va_end(va);
        return count;
    }
    virtual ssize_t InFV(FILE* in, char_t* chars, va_list va) {
        ssize_t count = 0;
        return count;
    }
    virtual ssize_t InLn(FILE* in, char_t* chars, size_t size) {
        ssize_t count = 0;
        return count;
    }
    virtual ssize_t InLn(FILE* in) {
        ssize_t count = 0;
        return count;
    }
    virtual ssize_t In(FILE* in, char_t* chars, size_t size) {
        ssize_t count = 0;
        if ((in) && (chars) && (size)) {
            ssize_t amount = 0;
            if ((amount = In(in, chars, sizeof(char_t), size))) {
                count += amount;
            } else {
                return amount;
            }
        }
        return count;
    }
    virtual ssize_t In(FILE* in, char_t* chars, size_t size, size_t length) {
        ssize_t count = 0;
        if ((in) && (chars) && (size) && (length)) {
            ssize_t amount = 0;
            if ((amount = ::fread(chars, size, length, in))) {
                count += amount;
            } else {
                return amount;
            }
        }
        return count;
    }

    virtual FILE* OutStdErr() {
        return StdErr();
    }
    virtual FILE* OutStdOut() {
        return StdOut();
    }
    virtual FILE* InStdIn() {
        return StdIn();
    }

    virtual FILE* StdErr() const {
        return stderr;
    }
    virtual FILE* StdOut() const {
        return stdout;
    }
    virtual FILE* StdIn() const {
        return stdin;
    }

    static IoT& TheIo(IoT& thisIo) {
        IoT* theIo = TheIo();
        if (theIo) {
            return *theIo;
        }
        return thisIo;
    }
protected:
    static IoT*& TheIo() {
        static IoT* theIo = 0;
        return theIo;
    }
};
typedef IoT<> Io;

namespace base {
typedef console::Io IoTImplements;
typedef Base IoTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: IoT
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = IoTImplements, class TExtends = IoTExtends>

class _EXPORT_CLASS IoT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Implements::string_t string_t;
    typedef typename Implements::chars_t chars_t;
    typedef typename Implements::char_t char_t;
    typedef typename Implements::endchar_t endchar_t;
    static const endchar_t endchar = Implements::endchar;
    typedef const char_t* const_chars_t;

    IoT(Locked& locked): _locked(locked) {
    }
    IoT(): _locked(*this) {
    }
    virtual ~IoT() {
    }
private:
    IoT(const IoT &copy): _locked(*this) {
    }
    
protected:
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
};
typedef IoT<> Io;
} /// namespace base

} /// namespace console
} /// namespace xos

#endif /// _XOS_CONSOLE_IO_HXX_
