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
///   File: thread.hxx
///
/// Author: $author$
///   Date: 6/4/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_PLATFORM_POSIX_MICROSOFT_WINDOWS_THREAD_HXX_
#define _XOS_PLATFORM_POSIX_MICROSOFT_WINDOWS_THREAD_HXX_

#include "xos/mt/posix/Thread.hxx"
#include "xos/platform/Posix.hxx"
#include "xos/platform/posix/microsoft/windows/thread.h"

namespace xos {
namespace platform {
namespace posix {
namespace microsoft {
namespace windows {

namespace internal {
template 
<typename TStartAddress, 
 typename TParameter,
 typename TResult, 
 class TExtends = mt::posix::Thread>
///////////////////////////////////////////////////////////////////////
///  Class: ThreadT
///////////////////////////////////////////////////////////////////////
class _EXPORTED_ ThreadT: public TExtends {
public:
    typedef TExtends Extends;
    typedef TStartAddress StartAddress;
    typedef TParameter Parameter;
    typedef TResult Result;
    typedef typename Extends::Attached Attached;
    typedef typename Extends::UnattachedT UnattachedT;
    static const UnattachedT Unattached = Extends::Unattached;
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ThreadT(StartAddress startAddress, Parameter parameter)
    : Extends(((Attached)Unattached), false, false), 
      _startAddress(startAddress), _parameter(parameter) {
    }
private:
    ThreadT(const ThreadT& copy) {
        XOS_MT_THREAD_UNEXPECTED();
    }
protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual void Run() {
        if ((_startAddress)) {
            Result result = (*_startAddress)(_parameter);
        }
    }
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    StartAddress _startAddress;
    Parameter _parameter;
}; /// class _EXPORTED_ ThreadT
typedef DWORD (WINAPI *StartAddress)(LPVOID);
typedef ThreadT<StartAddress, LPVOID, DWORD> Thread;
} /// namespace internal

///////////////////////////////////////////////////////////////////////
///  Class: ThreadT
///////////////////////////////////////////////////////////////////////
template 
<class TThread, class TImplements = Handle::Implements, class TExtends = Handle>
class _EXPORTED_ ThreadT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;
    typedef typename TThread::StartAddress StartAddress;
    typedef typename TThread::Parameter Parameter;
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ThreadT(StartAddress startAddress, Parameter parameter, bool initiallySuspeneded)
    : _thread(startAddress, parameter) {
        if (!(_thread.Create(initiallySuspeneded))) {
            throw CreateException(CreateFailed);
        }
    }
    ThreadT(StartAddress startAddress, Parameter parameter)
    : _thread(startAddress, parameter) {
        if (!(_thread.Create())) {
            throw CreateException(CreateFailed);
        }
    }
    virtual ~ThreadT() {
    }
private:
    ThreadT(const Thread& copy) {
        XOS_MT_THREAD_UNEXPECTED();
    }
public:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual BOOL CloseHandle() {
        BOOL success = (_thread.Destroy())?(TRUE):(FALSE);
        return success;
    }
    virtual DWORD WaitForSingleObject(DWORD dwMilliseconds) {
        DWORD dwStatus = WAIT_FAILED;
        ::xos::JoinStatus status = ::xos::JoinFailed;

        if (INFINITE != (dwMilliseconds)) {
            if (0 < (dwMilliseconds)) {
                status = _thread.TimedJoin(dwMilliseconds);
            } else {
                status = _thread.TryJoin();
            }
        } else {
            status = _thread.UntimedJoin();
        }
        switch (status) {
        case ::xos::JoinSuccess:
            dwStatus = WAIT_OBJECT_0;
            break;
        case ::xos::JoinBusy:
            dwStatus = WAIT_TIMEOUT;
            break;
        case ::xos::JoinInterrupted:
            dwStatus = WAIT_ABANDONED;
            break;
        }
        return dwStatus;
    }
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    TThread _thread;
}; /// class _EXPORTED_ ThreadT

typedef ThreadT<internal::Thread> ThreadExtends;
typedef ThreadExtends::Implements ThreadImplements;
///////////////////////////////////////////////////////////////////////
///  Class: Thread
///////////////////////////////////////////////////////////////////////
class _EXPORTED_ Thread: virtual public ThreadImplements, public ThreadExtends {
public:
    typedef ThreadImplements Implements;
    typedef ThreadExtends Extends;
    typedef typename Extends::StartAddress StartAddress;
    typedef typename Extends::Parameter Parameter;
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    Thread(StartAddress startAddress, Parameter parameter, bool initiallySuspeneded)
    : Extends(startAddress, parameter, initiallySuspeneded) {
    }
    Thread(StartAddress startAddress, Parameter parameter)
    : Extends(startAddress, parameter) {
    }
    virtual ~Thread() {
    }
private:
    Thread(const Thread& copy): Extends(NULL, NULL) {
        XOS_MT_THREAD_UNEXPECTED();
    }
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
}; /// class _EXPORTED_ Thread

namespace crt {
namespace internal {
/*
typedef mt::posix::Thread ThreadExtends;
class _EXPORTED_ Thread: public ThreadExtends {
public:
    typedef ThreadExtends Extends;
    Thread(unsigned ( __stdcall *startAddress )( void * ), void *arglist)
    : Extends(((Attached)Unattached), false, false), 
      _startAddress(startAddress), _arglist(arglist) {
    }
private:
    Thread(const Thread& copy) {
        XOS_MT_THREAD_UNEXPECTED();
    }
protected:
    virtual void Run() {
        if ((_startAddress)) {
            unsigned result = (*_startAddress)(_arglist);
        }
    }
    unsigned ( __stdcall *_startAddress )( void * );
    void *_arglist;
}; /// class _EXPORTED_ Thread
*/
typedef unsigned (__stdcall *StartAddress)(void*);
typedef windows::internal::ThreadT<StartAddress, void*, unsigned> Thread;
} /// namespace internal

/*
typedef Handle::Implements ThreadImplements;
typedef Handle ThreadExtends;
///////////////////////////////////////////////////////////////////////
///  Class: Thread
///////////////////////////////////////////////////////////////////////
class _EXPORTED_ Thread: virtual public ThreadImplements, public ThreadExtends {
public:
    typedef ThreadImplements Implements;
    typedef ThreadExtends Extends;

    Thread(unsigned ( __stdcall *startAddress )( void * ), void *arglist, bool initiallySuspeneded)
    : _thread(startAddress, arglist) {
        if (!(_thread.Create(initiallySuspeneded))) {
            throw CreateException(CreateFailed);
        }
    }
    Thread(unsigned ( __stdcall *startAddress )( void * ), void *arglist)
    : _thread(startAddress, arglist) {
        if (!(_thread.Create())) {
            throw CreateException(CreateFailed);
        }
    }
    virtual ~Thread() {
    }
private:
    Thread(const Thread& copy): _thread(NULL, NULL) {
        XOS_MT_THREAD_UNEXPECTED();
    }

public:
    virtual BOOL CloseHandle() {
        BOOL success = (_thread.Destroy())?(TRUE):(FALSE);
        return success;
    }
    virtual DWORD WaitForSingleObject(DWORD dwMilliseconds) {
        DWORD dwStatus = WAIT_FAILED;
        ::xos::JoinStatus status = ::xos::JoinFailed;

        if (INFINITE != (dwMilliseconds)) {
            if (0 < (dwMilliseconds)) {
                status = _thread.TimedJoin(dwMilliseconds);
            } else {
                status = _thread.TryJoin();
            }
        } else {
            status = _thread.UntimedJoin();
        }
        switch (status) {
        case ::xos::JoinSuccess:
            dwStatus = WAIT_OBJECT_0;
            break;
        case ::xos::JoinBusy:
            dwStatus = WAIT_TIMEOUT;
            break;
        case ::xos::JoinInterrupted:
            dwStatus = WAIT_ABANDONED;
            break;
        }
        return dwStatus;
    }
    virtual crt::Thread* ToCrtThread() const {
        return ((crt::Thread*)this);
    }

protected:
    internal::Thread _thread;
}; /// class _EXPORTED_ Thread
*/
typedef ThreadT<internal::Thread> ThreadExtends;
typedef ThreadExtends::Implements ThreadImplements;
///////////////////////////////////////////////////////////////////////
///  Class: Thread
///////////////////////////////////////////////////////////////////////
class _EXPORTED_ Thread: virtual public ThreadImplements, public ThreadExtends {
public:
    typedef ThreadImplements Implements;
    typedef ThreadExtends Extends;
    typedef typename Extends::StartAddress StartAddress;
    typedef typename Extends::Parameter Parameter;
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    Thread(StartAddress startAddress, Parameter parameter, bool initiallySuspeneded)
    : Extends(startAddress, parameter, initiallySuspeneded) {
    }
    Thread(StartAddress startAddress, Parameter parameter)
    : Extends(startAddress, parameter) {
    }
    virtual ~Thread() {
    }
private:
    Thread(const Thread& copy): Extends(NULL, NULL) {
        XOS_MT_THREAD_UNEXPECTED();
    }
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
}; /// class _EXPORTED_ Thread
} /// namespace crt

} /// namespace windows
} /// namespace microsoft
} /// namespace posix
} /// namespace platform
} /// namespace xos

#endif /// _XOS_PLATFORM_POSIX_MICROSOFT_WINDOWS_THREAD_HXX_
