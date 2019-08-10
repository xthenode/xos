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
///   File: Mutex.hxx
///
/// Author: $author$
///   Date: 6/21/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_MICROSOFT_WINDOWS_MUTEX_HXX_
#define _XOS_MT_MICROSOFT_WINDOWS_MUTEX_HXX_

#include "xos/mt/os/Os.hxx"
#include "xos/mt/Mutex.hxx"

namespace xos {
namespace mt {
namespace microsoft {
namespace windows {

///////////////////////////////////////////////////////////////////////
///  Class: MutexT
///////////////////////////////////////////////////////////////////////
template 
<typename TError = int, TError VErrorSuccess = 0, TError VErrorFailed = -1,
 typename TAttached = HANDLE, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::MutexT<TAttached, TUnattached, VUnattached>,
 class TMutex = extended::MutexT
 <TError, VErrorSuccess, VErrorFailed, TAttached, TUnattached, VUnattached, TImplement>,
 class TImplements = TImplement, class TExtends = TMutex>

class _EXPORT_CLASS MutexT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Extends::Attached Attached;
    static const typename Extends::UnattachedT Unattached = Extends::Unattached;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    MutexT(Attached detached, bool isCreated, bool isLogged, bool isErrLogged): Extends(detached, isCreated, isLogged, isErrLogged) {
    }
    MutexT(Attached detached, bool isCreated, bool isLogged): Extends(detached, isCreated, isLogged) {
    }
    MutexT(Attached detached, bool isCreated): Extends(detached, isCreated) {
    }
    MutexT(Attached detached): Extends(detached) {
    }
    MutexT(bool isLogged, bool isErrLogged): Extends(isLogged, isErrLogged) {
        XOS_MT_MUTEX_CREATED();
    }
    MutexT(bool isLogged): Extends(isLogged) {
        XOS_MT_MUTEX_CREATED();
    }
    MutexT() {
        XOS_MT_MUTEX_CREATED();
    }
    MutexT(const MutexT &copy): Extends(copy.AttachedTo(), false) {
    }
    virtual ~MutexT() {
        XOS_MT_MUTEX_DESTROYED();
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool Lock() { 
        return LockDetached(this->AttachedTo());
    }
    virtual bool Unlock() { 
        return UnlockDetached(this->AttachedTo());
    }
    virtual LockStatus TimedLock(mseconds_t milliseconds) { 
        return TimedLockDetached(this->AttachedTo(), milliseconds);
    }
    virtual LockStatus TryLock() { 
        return TryLockDetached(this->AttachedTo());
    }
    virtual LockStatus UntimedLock() { 
        return UntimedLockDetached(this->AttachedTo());
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool LockDetached(Attached detached) const { 
        if (LockSuccess != (UntimedLockDetached(detached))) {
            return false; 
        }
        return true; 
    }
    virtual bool UnlockDetached(Attached detached) const { 
        if (((Attached)Unattached) != (detached)) {
            IS_ERR_LOGGED_DEBUG("::ReleaseMutex(detached)...");
            if ((::ReleaseMutex(detached))) {
                IS_ERR_LOGGED_DEBUG("...::ReleaseMutex(detached)");
                return true;
            } else {
                DWORD dwError  = GetLastError();
                IS_ERR_LOGGED_ERROR("...failed dwError = " << dwError << " on ::ReleaseMutex(detached)");
            }
        }
        return false; 
    }
    virtual LockStatus TimedLockDetached(Attached detached, mseconds_t milliseconds) const { 
        if (0 > (milliseconds)) {
            return UntimedLockDetached(detached);
        } else {
            if (((Attached)Unattached) != (detached)) {
                bool isLogged = ((this->IsLogged()) && (milliseconds >= this->TimedLoggedThreasholdMilliseconds()));
                DWORD dwMilliseconds = (DWORD)(milliseconds), dwResult = 0;
                
                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "WaitForSingleObject(detached, dwMilliseconds)...");
                if (WAIT_OBJECT_0 != (dwResult = WaitForSingleObject(detached, dwMilliseconds))) {
                    switch(dwResult) {
                    case WAIT_TIMEOUT:
                        IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...failed WAIT_TIMEOUT dwResult = " << dwResult << " on WaitForSingleObject(detached, dwMilliseconds)...");
                        return LockBusy;
                    case WAIT_ABANDONED:
                        IS_ERR_LOGGED_ERROR("...failed WAIT_ABANDONED dwResult = " << dwResult << " on WaitForSingleObject(detached, dwMilliseconds)...");
                        return LockInterrupted;
                    default:
                        IS_ERR_LOGGED_ERROR("...failed dwResult = " << dwResult << " on WaitForSingleObject(detached, dwMilliseconds)...");
                        return LockFailed;
                    }
                } else {
                    IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...WaitForSingleObject(detached, dwMilliseconds)");
                    return LockSuccess;
                }
            }
        }
        return LockFailed; 
    }
    virtual LockStatus TryLockDetached(Attached detached) const { 
        return TimedLockDetached(detached, 0); 
    }
    virtual LockStatus UntimedLockDetached(Attached detached) const { 
        if (((Attached)Unattached) != (detached)) {
            DWORD dwMilliseconds = INFINITE, dwResult = 0;

            IS_ERR_LOGGED_DEBUG("WaitForSingleObject(detached, dwMilliseconds)...");
            if (WAIT_OBJECT_0 != (dwResult = WaitForSingleObject(detached, dwMilliseconds))) {
                switch(dwResult) {
                case WAIT_TIMEOUT:
                    IS_ERR_LOGGED_ERROR("...failed WAIT_TIMEOUT dwResult = " << dwResult << " on WaitForSingleObject(detached, dwMilliseconds)...");
                    return LockBusy;
                case WAIT_ABANDONED:
                    IS_ERR_LOGGED_ERROR("...failed WAIT_ABANDONED dwResult = " << dwResult << " on WaitForSingleObject(detached, dwMilliseconds)...");
                    return LockInterrupted;
                default:
                    IS_ERR_LOGGED_ERROR("...failed dwResult = " << dwResult << " on WaitForSingleObject(detached, dwMilliseconds)...");
                    return LockFailed;
                }
            } else {
                IS_ERR_LOGGED_DEBUG("...WaitForSingleObject(detached, dwMilliseconds)");
                return LockSuccess;
            }
        }
        return LockFailed; 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Attached CreateDetached() const {
        return CreateDetached(false);
    }
    virtual Attached CreateDetached(bool initiallyLocked) const {
        Attached detached = ((Attached)Unattached);
        BOOL bInitialOwner = (initiallyLocked)?(TRUE):(FALSE);
        LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL;
        LPCTSTR lpName = NULL;

        IS_ERR_LOGGED_DEBUG("::CreateMutex(lpMutexAttributes, bInitialOwner, lpName)...");
        if ((detached = ::CreateMutex(lpMutexAttributes, bInitialOwner, lpName))) {
            IS_ERR_LOGGED_DEBUG("...::CreateMutex(lpMutexAttributes, bInitialOwner, lpName)");
        } else {
            DWORD dwError = GetLastError();
            IS_ERR_LOGGED_ERROR("...failed dwError = "<< dwError << " on ::CreateMutex(lpMutexAttributes, bInitialOwner, lpName)");
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
}; /// class _EXPORT_CLASS MutexT
typedef MutexT<> Mutex;
    
} /// namespace windows
} /// namespace microsoft
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_MICROSOFT_WINDOWS_MUTEX_HXX_
