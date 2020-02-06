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
///   File: Allocated.hxx
///
/// Author: $author$
///   Date: 5/5/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_BASE_ALLOCATED_HXX_
#define _XOS_BASE_ALLOCATED_HXX_

#include "xos/base/Exception.hxx"

namespace xos {

typedef int AllocateStatus;
enum {
    FreeSuccess,
    AllocateSuccess,
    AllocateFailed,
    FreeFailed,
    AllocateBusy,
    AllocateInterrupted,
    AllocateInvalid,
    FreeBusy,
    FreeInterrupted,
    FreeInvalid
};
template <class TString>
inline const char* AllocateStatusToString(AllocateStatus status) {
    switch (status) {
    case FreeSuccess: return TString("FreeSuccess");
    case AllocateSuccess: return TString("AllocateSuccess");
    case AllocateFailed: return TString("AllocateFailed");
    case FreeFailed: return TString("FreeFailed");
    case AllocateBusy: return TString("AllocateBusy");
    case AllocateInterrupted: return TString("AllocateInterrupted");
    case AllocateInvalid: return TString("AllocateInvalid");
    case FreeBusy: return TString("FreeBusy");
    case FreeInterrupted: return TString("FreeInterrupted");
    case FreeInvalid: return TString("FreeInvalid");
    default: break;
    }
    return TString("Unknown");
}
///////////////////////////////////////////////////////////////////////
///  Class: AllocateExceptionT
///////////////////////////////////////////////////////////////////////
template 
<typename TStatus = AllocateStatus, 
 class TString = String, 
 class TImplements = Exception::Implements, 
 class TExtends = ExceptionT<TStatus, TString, TImplements> >

class _EXPORTED_ AllocateExceptionT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef AllocateExceptionT Derives;

    typedef TStatus status_t;
    typedef TString string_t;
    
    AllocateExceptionT(const status_t& status): Extends(status) {
    }
    AllocateExceptionT(const Derives &copy): Extends(copy.Status()) {
    }
    AllocateExceptionT(): Extends(AllocateFailed) {
    }
    virtual ~AllocateExceptionT() {
    }

    virtual string_t StatusToString() const {
        return ExceptionStatusToString<string_t>(this->Status());
    }
};
typedef AllocateExceptionT<> AllocateException;

typedef ImplementBase AllocatorTImplements;
///////////////////////////////////////////////////////////////////////
///  Class: AllocatorT
///////////////////////////////////////////////////////////////////////
template <class TImplements = AllocatorTImplements>
class _EXPORTED_ AllocatorT: virtual public TImplements {
public:
    typedef TImplements Implements;
};
typedef AllocatorT<> Allocator;

typedef Allocator AllocatedTImplements;
typedef Base AllocatedTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: AllocatedT
///////////////////////////////////////////////////////////////////////
template <class TImplements = AllocatedTImplements, class TExtends = AllocatedTExtends>
class _EXPORTED_ AllocatedT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    AllocatedT(const AllocatedT &copy) {
    }
    AllocatedT() {
    }
    virtual ~AllocatedT() {
    }
};
typedef AllocatedT<> Allocated;

} /// namespace xos

#endif /// _XOS_BASE_ALLOCATED_HXX_
