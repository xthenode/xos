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
///   Date: 7/7/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_MICROSOFT_WINDOWS_THREAD_HXX_
#define _XOS_MT_MICROSOFT_WINDOWS_THREAD_HXX_

#include "xos/mt/Thread.hxx"

namespace xos {
namespace mt {
namespace microsoft {
namespace windows {

///////////////////////////////////////////////////////////////////////
///  Class: ThreadT
///////////////////////////////////////////////////////////////////////
template 
<typename TTID = DWORD, typename TError = int, TError VErrorSuccess = 0, TError VErrorFailed = -1,
 typename TAttached = HANDLE, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::ThreadT<TAttached, TUnattached, VUnattached>,
 class TThread = extended::ThreadT
 <TError, VErrorSuccess, VErrorFailed, TAttached, TUnattached, VUnattached, TImplement>,
 class TImplements = TImplement, class TExtends = TThread>

class _EXPORTED_ ThreadT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef TTID TID;
    typedef typename Implements::Ran Ran;

    typedef typename Extends::Error Error;
    static const Error ErrorSuccess = Extends::ErrorSuccess;
    static const Error ErrorFailed = Extends::ErrorFailed;

    typedef typename Extends::Attached Attached;
    static const typename Extends::UnattachedT Unattached = Extends::Unattached;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ThreadT(Attached detached, bool isCreated, bool isLogged, bool isErrLogged): Extends(detached, isCreated, isLogged, isErrLogged) {
    }
    ThreadT(Attached detached, bool isCreated, bool isLogged): Extends(detached, isCreated, isLogged) {
    }
    ThreadT(Attached detached, bool isCreated): Extends(detached, isCreated) {
    }
    ThreadT(Attached detached): Extends(detached) {
    }
    ThreadT(bool isLogged, bool isErrLogged): Extends(isLogged, isErrLogged) {
        XOS_MT_THREAD_CREATED();
    }
    ThreadT(bool isLogged): Extends(isLogged) {
        XOS_MT_THREAD_CREATED();
    }
    ThreadT(Ran& ran): Extends(ran) {
        XOS_MT_THREAD_CREATED();
    }
    ThreadT() {
        XOS_MT_THREAD_CREATED();
    }
    virtual ~ThreadT() {
        XOS_MT_THREAD_DESTROYED();
    }
private:
    ThreadT(const ThreadT& copy) {
        XOS_MT_THREAD_UNEXPECTED();
    }
    
public:    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Attached CreateAttached() {
        return CreateAttached(false);
    }
    virtual Attached CreateAttached(bool initiallySuspended) {
        Attached detached = ((Attached)Unattached);
        IS_ERR_LOGGED_DEBUG("CreateDetached(initiallySuspended)...");
        if ((detached = CreateDetached(_threadId, initiallySuspended))) {
            this->Attach(detached);
        }
        return detached;
    }
    virtual Attached DetachCreated(bool& isCreated) {
        IS_ERR_LOGGED_DEBUG("this->SetThreadId(0)...");
        this->SetThreadId(0);
        return Extends::DetachCreated(isCreated);
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual JoinStatus TryJoinDetached(bool& forked, Attached detached) const { 
        return TimedJoinDetached(forked, detached, 0); 
    }
    virtual JoinStatus TimedJoinDetached(bool& forked, Attached detached, mseconds_t milliseconds) const { 
        if (0 > (milliseconds)) {
            return UntimedJoinDetached(forked, detached);
        } else {
            if (((Attached)Unattached) != (detached)) {
                bool isLogged = ((this->IsLogged()) && (milliseconds >= this->TimedLoggedThreasholdMilliseconds()));
                DWORD dwMilliseconds = ((DWORD)milliseconds), dwResult = 0;

                IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "::WaitForSingleObject(detached, dwMilliseconds)...");
                if (WAIT_OBJECT_0 != (dwResult = ::WaitForSingleObject(detached, dwMilliseconds))) {
                    switch(dwResult) {
                    case WAIT_TIMEOUT:
                        IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "...WAIT_TIMEOUT dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)...");
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
                    IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "...::WaitForSingleObject(detached, dwMilliseconds)");
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

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Attached CreateDetached(TID& threadId, bool initiallySuspended) const {
        Attached detached = ((Attached)Unattached);
        LPTHREAD_START_ROUTINE lpStartAddress = ThreadProc;
        LPVOID lpParameter = ((LPVOID)((ThreadT*)this));
        DWORD dwCreationFlags = (initiallySuspended)?(CREATE_SUSPENDED):(0);
        DWORD dwStackSize = 0;
        DWORD dwThreadId = 0;
        SECURITY_ATTRIBUTES lpThreadAttributes;

        memset(&lpThreadAttributes, 0, sizeof(lpThreadAttributes));
        lpThreadAttributes.nLength = sizeof(lpThreadAttributes);

        IS_ERR_LOGGED_DEBUG("::CreateThread(&lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, &dwThreadId)...");
        if ((detached = ::CreateThread(&lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, &dwThreadId))) {
            IS_ERR_LOGGED_DEBUG("...::CreateThread(&lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, &dwThreadId)");
            threadId = dwThreadId;
        } else {
            DWORD dwError = GetLastError();
            IS_ERR_LOGGED_ERROR("...failed dwError = " << dwError << " on ::CreateThread(&lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, &dwThreadId)...");
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
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual TID SetThreadId(TID to) {
        _threadId = to;
        return _threadId;
    }
    virtual TID ThreadId() const {
        return _threadId;
    }

protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    static DWORD WINAPI ThreadProc(LPVOID lpParameter) {
        DWORD dwResult = 0;
        ThreadT* t = 0;
        LOG_DEBUG("in...");
        if ((t = ((ThreadT*)lpParameter))) {
            IF_ERR_LOGGED_DEBUG(t->IsLogged(), t->IsErrLogged(), "t->Run()...");
            t->Run();
            IF_ERR_LOGGED_DEBUG(t->IsLogged(), t->IsErrLogged(), "...t->Run()");
        } else {
            LOG_ERROR("...unexpected 0 = ((ThreadT*)param)");
        }
        LOG_DEBUG("...out");
        return dwResult;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    TID _threadId;
}; /// class _EXPORTED_ ThreadT
typedef ThreadT<> Thread;

} /// namespace windows
} /// namespace microsoft
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_MICROSOFT_WINDOWS_THREAD_HXX_
