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
///   File: Thread.hxx
///
/// Author: $author$
///   Date: 6/3/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_MICROSOFT_WINDOWS_CRT_THREAD_HXX_
#define _XOS_MT_MICROSOFT_WINDOWS_CRT_THREAD_HXX_

#include "xos/mt/os/Os.hxx"
#include "xos/mt/Thread.hxx"

#include <process.h>
#include <errno.h>

namespace xos {
namespace mt {
namespace microsoft {
namespace windows {
namespace crt {

typedef int ThreadTError;
static const ThreadTError ThreadVErrorSuccess = 0;
typedef HANDLE ThreadTAttached;
typedef int ThreadTUnattached;
static const ThreadTUnattached ThreadVUnattached = -1;
///////////////////////////////////////////////////////////////////////
///  Class: ThreadT
///////////////////////////////////////////////////////////////////////
template 
<class TImplement = implemented::ThreadT
 <ThreadTAttached, ThreadTUnattached, ThreadVUnattached>,
 class TThread = extended::ThreadT
 <ThreadTError, ThreadVErrorSuccess, 
  ThreadTAttached, ThreadTUnattached, ThreadVUnattached, TImplement>,
 class TImplements = TImplement, class TExtends = TThread>

class _EXPORT_CLASS ThreadT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Extends::Ran Ran;

    typedef typename Extends::error_t error_t;
    static const error_t ErrorSuccess = Extends::ErrorSuccess;

    typedef typename Extends::Attached Attached;
    static const typename Extends::UnattachedT Unattached = Extends::Unattached;
    
    ThreadT(Attached detached, bool isCreated, bool isLogged, bool isErrLogged)
    : Extends(detached, isCreated, isLogged, isErrLogged), _threadId(0) {
    }
    ThreadT(Attached detached, bool isCreated, bool isLogged)
    : Extends(detached, isCreated, isLogged), _threadId(0) {
    }
    ThreadT(Attached detached, bool isCreated): Extends(detached, isCreated), _threadId(0) {
    }
    ThreadT(Attached detached): Extends(detached), _threadId(0) {
    }
    ThreadT(bool isLogged, bool isErrLogged): Extends(isLogged, isErrLogged), _threadId(0) {
        XOS_MT_THREAD_CREATED(false);
    }
    ThreadT(bool isLogged): Extends(isLogged), _threadId(0) {
        XOS_MT_THREAD_CREATED(false);
    }
    ThreadT(Ran& ran): Extends(ran), _threadId(0) {
        XOS_MT_THREAD_CREATED(false);
    }
    ThreadT(): _threadId(0) {
        XOS_MT_THREAD_CREATED(false);
    }
    virtual ~ThreadT() {
        XOS_MT_THREAD_DESTROYED();
    }
private:
    ThreadT(const ThreadT &copy) {
        XOS_MT_THREAD_UNEXPECTED();
    }

public:    
    virtual Attached CreateDetached() const {
        return CreateDetached(false);
    }
    virtual Attached CreateDetached(bool initiallySuspended) const {
        return CreateDetached((unsigned&)_threadId, initiallySuspended);
    }
    virtual Attached CreateDetached(unsigned& threadId, bool initiallySuspended) const {
        Attached detached = ((Attached)Unattached);
        unsigned (__stdcall*startAddress)(void*) = ThreadProc;
        void* parameter = ((void*)this);
        unsigned initFlags = (initiallySuspended)?(CREATE_SUSPENDED):(0);
        unsigned stackSize = 0;
        SECURITY_ATTRIBUTES securityAttributes;

        ::memset(&securityAttributes, 0, sizeof(securityAttributes));
        securityAttributes.nLength = sizeof(securityAttributes);

        IS_ERR_LOGGED_DEBUG("::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &threadId)...");
        if (((Attached)Unattached) != (detached = ((Attached)::_beginthreadex
            (&securityAttributes, stackSize, startAddress, parameter, initFlags, &threadId)))) {
            IS_ERR_LOGGED_DEBUG("...::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &threadId)");
        } else {
            int err = errno;
            switch(err) {
            case EACCES:
                IS_ERR_LOGGED_ERROR("...EACCES err = " << err << " on ::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &thread_id)");
                break;
            case EAGAIN:
                IS_ERR_LOGGED_ERROR("...EAGAIN err = " << err << " on ::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &thread_id)");
                break;
            case EINVAL:
                IS_ERR_LOGGED_ERROR("...EINVAL err = " << err << " on ::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &thread_id)");
                break;
            default:
                IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::_beginthreadex(&securityAttributes, stackSize, startAddress, parameter, initFlags, &thread_id)");
            }
        }
        return detached;
    }
    virtual bool DestroyDetached(Attached detached) const {
        if (((Attached)Unattached) != detached) {
            IS_ERR_LOGGED_DEBUG("::CloseHandle(detached)...");
            if ((::CloseHandle(detached))) {
                IS_ERR_LOGGED_DEBUG("...::CloseHandle(detached)");
                return true;
            } else {
                DWORD dwError = ::GetLastError();
                IS_ERR_LOGGED_ERROR("...failed dwError = " << dwError << " on ::CloseHandle(detached)");
            }
        }
        return false;
    }

    virtual JoinStatus TryJoinDetached(bool& forked, Attached detached) const { 
        return TimedJoinDetached(forked, detached, 0); 
    }
    virtual JoinStatus TimedJoinDetached(bool& forked, Attached detached, mseconds_t milliseconds) const { 
        if (0 > (milliseconds)) {
            return UntimedJoinDetached(forked, detached);
        } else {
            if (((Attached)Unattached) != (detached)) {
                DWORD dwMilliseconds = ((DWORD)milliseconds), dwResult = 0;

                IS_ERR_LOGGED_TRACE("::WaitForSingleObject(detached, dwMilliseconds)...");
                if (WAIT_OBJECT_0 != (dwResult = ::WaitForSingleObject(detached, dwMilliseconds))) {
                    switch(dwResult) {
                    case WAIT_TIMEOUT:
                        IS_ERR_LOGGED_TRACE("...WAIT_TIMEOUT dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                        return JoinBusy;
                    case WAIT_ABANDONED:
                        IS_ERR_LOGGED_ERROR("...WAIT_ABANDONED dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                        return JoinInterrupted;
                    default:
                        IS_ERR_LOGGED_ERROR("...failed dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                        forked = false;
                        return JoinFailed;
                    }
                } else {
                    IS_ERR_LOGGED_DEBUG("...::WaitForSingleObject(detached, dwMilliseconds)");
                    forked = false;
                    return JoinSuccess;
                }
            }
        }
        return JoinFailed; 
    }
    virtual JoinStatus UntimedJoinDetached(bool& forked, Attached detached) const { 
        if (((Attached)Unattached) != (detached)) {
            DWORD dwMilliseconds = ((DWORD)INFINITE), dwResult = 0;

            IS_ERR_LOGGED_DEBUG("::WaitForSingleObject(detached, dwMilliseconds)...");
            if (WAIT_OBJECT_0 != (dwResult = ::WaitForSingleObject(detached, dwMilliseconds))) {
                switch(dwResult) {
                case WAIT_TIMEOUT:
                    IS_ERR_LOGGED_ERROR("...WAIT_TIMEOUT dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                    return JoinBusy;
                case WAIT_ABANDONED:
                    IS_ERR_LOGGED_ERROR("...WAIT_ABANDONED dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                    return JoinInterrupted;
                default:
                    IS_ERR_LOGGED_ERROR("...failed dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
                    forked = false;
                    return JoinFailed;
                }
            } else {
                IS_ERR_LOGGED_DEBUG("...::WaitForSingleObject(detached, dwMilliseconds)");
                forked = false;
                return JoinSuccess;
            }
        }
        return JoinFailed; 
    }

protected:
    static unsigned __stdcall ThreadProc(void* param) {
        unsigned result = 0;
        ThreadT* t = 0;
        if ((t = ((ThreadT*)param))) {
            IF_ERR_LOGGED_DEBUG(t->IsLogged(), t->IsErrLogged(), "t->Run()...");
            t->Run();
            IF_ERR_LOGGED_DEBUG(t->IsLogged(), t->IsErrLogged(), "...t->Run()");
        } else {
            LOG_ERROR("...unexpected 0 = ((ThreadT*)param)");
        }
        return result;
    }

protected:
    unsigned _threadId;
}; /// class _EXPORT_CLASS ThreadT
typedef ThreadT<> Thread;

} /// namespace crt
} /// namespace windows
} /// namespace microsoft
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_MICROSOFT_WINDOWS_CRT_THREAD_HXX_
