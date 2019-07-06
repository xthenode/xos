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
///   File: Version.hxx
///
/// Author: $author$
///   Date: 5/6/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_LIB_VERSION_HXX_
#define _XOS_LIB_VERSION_HXX_

#include "xos/base/String.hxx"

namespace xos {
namespace lib {

typedef ImplementBase VersionTImplements;
///////////////////////////////////////////////////////////////////////
///  Class: VersionT
///////////////////////////////////////////////////////////////////////
template 
<class TChar = char, 
 class TString = StringT<TChar>,
 class TImplements = VersionTImplements>

class _EXPORT_CLASS VersionT: virtual public TImplements {
public:
    typedef TImplements Implements;
    typedef TString string_t;

    virtual string_t ToString() const {
        string_t to(Name());
        to.Append(NameSeparator());
        to.AppendUnsigned(Major());
        to.Append(MajorSeparator());
        to.AppendUnsigned(Minor());
        to.Append(MinorSeparator());
        to.AppendUnsigned(Release());
        return to;
    }

    virtual string_t Name() const {
        return string_t("version");
    }
    virtual string_t NameSeparator() const {
        return string_t("-");
    }
    virtual string_t MajorSeparator() const {
        return string_t(".");
    }
    virtual string_t MinorSeparator() const {
        return MajorSeparator();
    }

    virtual unsigned Major() const {
        return 0;
    }
    virtual unsigned Minor() const {
        return 0;
    }
    virtual unsigned Release() const {
        return 0;
    }
};
typedef VersionT<> Version;

} /// namespace lib
} /// namespace xos

#endif /// _XOS_LIB_VERSION_HXX_
