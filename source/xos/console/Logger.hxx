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
///   File: Logger.hxx
///
/// Author: $author$
///   Date: 5/6/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_CONSOLE_LOGGER_HXX_
#define _XOS_CONSOLE_LOGGER_HXX_

#include "xos/logger/Interface.hxx"
#include "xos/console/Io.hxx"

namespace xos {
namespace console {

typedef logger::Base::Implements LoggerTImplements;
typedef logger::Base LoggerTExtends;
///////////////////////////////////////////////////////////////////////
///  Class: LoggerT
///////////////////////////////////////////////////////////////////////
template 
<class TImplements = LoggerTImplements, class TExtends = LoggerTExtends>

class _EXPORT_CLASS LoggerT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Implements::char_t char_t;
    
    LoggerT(Io &io): Extends(io), _io(io) {
    }
    LoggerT(): _io(_thisIo) {
    }
    virtual ~LoggerT() {
    }
private:
    LoggerT(const LoggerT &copy) {
    }

protected:
    virtual void Out(const char_t& c) {
        _io.Err(&c, 1);
    }
    virtual void OutFlush() {
        _io.ErrFlush();
    }
    
protected:
    Io _thisIo, &_io;
};
typedef LoggerT<> Logger;

} /// namespace console
} /// namespace xos

#endif /// _XOS_CONSOLE_LOGGER_HXX_
