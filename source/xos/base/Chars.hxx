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
///   File: Chars.hxx
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_BASE_CHARS_HXX_
#define _XOS_BASE_CHARS_HXX_

#include "xos/base/Base.hxx"

namespace xos {

typedef ImplementBase CharsTImplements;
///////////////////////////////////////////////////////////////////////
///  Class: CharsT
///////////////////////////////////////////////////////////////////////
template 
<typename TChar = char,
 typename TEnd = TChar, TEnd VEnd = 0,
 class TImplements = CharsTImplements>
class _EXPORTED_ CharsT: virtual public TImplements {
public:
    typedef TImplements Implements;
    typedef CharsT Derives;

    typedef TChar char_t;
    typedef TEnd end_t;
    static const end_t endof = VEnd;
    typedef char_t (*to_case_t)(char_t c);

    class _EXPORTED_ from_signed_t {
    public:
        from_signed_t(signed from): _from(from) {}
        operator signed() const { return _from; }
    protected:
        signed _from;
    };
    class _EXPORTED_ from_unsigned_t {
    public:
        from_unsigned_t(unsigned from): _from(from) {}
        operator unsigned() const { return _from; }
    protected:
        unsigned _from;
    };

    static char_t* Copy
    (char_t* toWhat, const char_t* what) {
        return Copy(toWhat, what, -1, endof);
    }
    static char_t* Copy
    (char_t* toWhat, const char_t* what, ssize_t length) {
        return Copy(toWhat, what, length, endof);
    }
    static char_t* Copy
    (char_t* toWhat, const char_t* what,
     ssize_t length, end_t end) {
        if (toWhat) {
            char_t c = 0;
            if (0 > length) {
                while ((c = (*what++)) != end) {
                    (*toWhat++) = c;
                }
                (*toWhat) = c;
            } else {
                while (0 < length) {
                    (*toWhat++) = (c = (*what++));
                    --length;
                }
            }
        }
        return toWhat;
    }

    static int Compare(const char_t* what, const char_t* toWhat) {
        int unequal = CompareCased
        (what, -1, toWhat, -1, endof, endof, Derives::ToCase);
        return unequal;
    }
    static int Compare
    (const char_t* what, const char_t* toWhat, ssize_t length) {
        int unequal = CompareCased
        (what, length, toWhat, length, endof, endof, Derives::ToCase);
        return unequal;
    }
    static int Compare
    (const char_t* what, ssize_t length,
     const char_t* toWhat, ssize_t toLength) {
        int unequal = CompareCased
        (what, length, toWhat, toLength, endof, endof, Derives::ToCase);
        return unequal;
    }

    static int CompareCased(const char_t* what, const char_t* toWhat) {
        int unequal = CompareCased
        (what, -1, toWhat, -1, endof, endof, Derives::ToCase);
        return unequal;
    }
    static int CompareCased
    (const char_t* what, const char_t* toWhat, ssize_t length) {
        int unequal = CompareCased
        (what, length, toWhat, length, endof, endof, Derives::ToCase);
        return unequal;
    }
    static int CompareCased
    (const char_t* what, ssize_t length,
     const char_t* toWhat, ssize_t toLength) {
        int unequal = CompareCased
        (what, length, toWhat, toLength, endof, endof, Derives::ToCase);
        return unequal;
    }

    static int CompareCased
    (const char_t* what, ssize_t length,
     const char_t* toWhat, ssize_t toLength,
     end_t end = endof, end_t toEnd = endof,
     to_case_t ToCase = Derives::ToCase) {
        int unequal = 0;
        char_t c1 = 0, c2 = 0;

        if (what != toWhat) {
            if (what) {
                if (toWhat) {
                    if (0 > length) {
                        if (0 > toLength) {
                            for (c1 = *what, c2 = *toWhat;
                                 ((c1) != end) && ((c2) != toEnd);
                                 ++what, ++toWhat, c1 = *what, c2 = *toWhat) {
                                if ((c1 = ToCase(c1)) > (c2 = ToCase(c2))) {
                                    return 1;
                                } else {
                                    if (c1 < c2) {
                                        return -1;
                                    }
                                }
                            }
                            if (c1 != end) {
                                unequal = 1;
                            } else {
                                if (c2 != toEnd) {
                                    unequal = -1;
                                }
                            }
                        } else {
                            for (c1 = *what; ((c1) != end) && (0 < toLength);
                                 --toLength, ++what, ++toWhat, c1 = *what) {
                                c2 = *toWhat;
                                if ((c1 = ToCase(c1)) > (c2 = ToCase(c2))) {
                                    return 1;
                                } else {
                                    if (c1 < c2) {
                                        return -1;
                                    }
                                }
                            }
                            if (c1 != end) {
                                unequal = 1;
                            } else {
                                if (0 < toLength) {
                                    unequal = -1;
                                }
                            }
                        }
                    } else {
                        if (0 > toLength) {
                            for (c2 = *toWhat; (0 < length) && ((c2) != toEnd);
                                 --length, ++what, ++toWhat, c2 = *toWhat) {
                                c1 = *what;
                                if ((c1 = ToCase(c1)) > (c2 = ToCase(c2))) {
                                    return 1;
                                } else {
                                    if (c1 < c2) {
                                        return -1;
                                    }
                                }
                            }
                            if (0 < length) {
                                unequal = 1;
                            } else {
                                if (c2 != toEnd) {
                                    unequal = -1;
                                }
                            }
                        } else {
                            if (length > toLength) {
                                unequal = 1;
                                length = toLength;
                            } else {
                                if (toLength > length) {
                                    unequal = -1;
                                }
                            }
                            for (; 0 < length; --length, ++what, ++toWhat) {
                                c1 = *what; c2 = *toWhat;
                                if ((c1 = ToCase(c1)) > (c2 = ToCase(c2))) {
                                    return 1;
                                } else {
                                    if (c1 < c2) {
                                        return -1;
                                    }
                                }
                            }
                        }
                    }
                } else {
                    unequal = 1;
                }
            } else {
                if (toWhat) {
                    unequal = -1;
                }
            }
        }
        return unequal;
    }

    static signed ToSigned
    (const char_t* what, ssize_t length = -1, end_t end = endof) {
        const char_t c0 = (char_t)('0');
        const char_t c9 = (char_t)('9');
        const char_t minus = (char_t)('-');
        bool negative = false;
        signed value = 0;
        size_t digits = 0, amount = 0;
        char_t c = 0, d = 0;

        if (what) {
            if (0 > length) {
                for (digits = 0, amount = 0; (c = what[amount]) != end; ++amount) {
                    if ((c >= c0) && (c <= c9)) {
                        value = (value*10) + (signed)(d = (char_t)(c - c0));
                        if ((d != 0) || digits)
                            ++digits;
                    } else {
                        if ((minus == c) && !(digits)) {
                            negative = !negative;
                        }
                    }
                }
            } else {
                for (digits = 0, amount = 0; amount < length; ++amount) {
                    if (((c = what[amount]) >= c0) && (c <= c9)) {
                        value = (value*10) + (signed)(d = (char_t)(c - c0));
                        if ((d != 0) || digits)
                            ++digits;
                    } else {
                        if ((minus == c) && !(digits)) {
                            negative = !negative;
                        }
                    }
                }
            }
        }
        if ((value) && (negative)) {
            value = -value;
        }
        return value;
    }
    static unsigned ToUnsigned
    (const char_t* what, ssize_t length = -1, end_t end = endof) {
        const char_t c0 = (char_t)('0');
        const char_t c9 = (char_t)('9');
        unsigned value = 0;
        size_t digits = 0, amount = 0;
        char_t c = 0, d = 0;

        if (what) {
            if (0 > length) {
                for (digits = 0, amount = 0; (c = what[amount]) != end; ++amount) {
                    if ((c >= c0) && (c <= c9)) {
                        value = (value*10) + (unsigned)(d = (char_t)(c - c0));
                        if ((d != 0) || digits)
                            ++digits;
                    }
                }
            } else {
                for (digits = 0, amount = 0; amount < length; ++amount) {
                    if (((c = what[amount]) >= c0) && (c <= c9)) {
                        value = (value*10) + (unsigned)(d = (char_t)(c - c0));
                        if ((d != 0) || digits)
                            ++digits;
                    }
                }
            }
        }
        return value;
    }

    static char_t ToLower(char_t c) {
        static const char_t A = ((char_t)'A');
        static const char_t Z = ((char_t)'Z');
        static const char_t a = ((char_t)'a');
        if ((c >= A) && (c <= Z)) {
            c = a + (c - A);
        }
        return c;
    }
    static char_t ToUpper(char_t c) {
        static const char_t A = ((char_t)'A');
        static const char_t a = ((char_t)'a');
        static const char_t z = ((char_t)'z');
        if ((c >= a) && (c <= z)) {
            c = A + (c - a);
        }
        return c;
    }
    static char_t ToCase(char_t c) {
        return c;
    }

    static char_t Tox(byte_t d, bool upper_case = false) {
        char a = (upper_case)?('A'):('a');
        char_t x = (char_t)(0);
        if ((0 <= d) && (9 >= d)) {
            x = (char_t)(('0') +  d);
        } else {
            if ((10 <= d) && (15 >= d)) {
                x = (char_t)((a) + (d - 10));
            }
        }
        return x;
    }

    static size_t Count(const char_t* chars, end_t end = endof) {
        size_t length = 0;
        if (chars) {
            char_t c = 0;
            for (char_t e = ((char_t)end);
                 ((c = *chars) != e); ++chars) {
                ++length;
            }
        }
        return length;
    }
};
typedef CharsT<char> Chars;

} /// namespace xos

#endif /// _XOS_BASE_CHARS_HXX_
