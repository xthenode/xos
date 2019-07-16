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
///   File: Semaphore.hxx
///
/// Author: $author$
///   Date: 6/23/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_MICROSOFT_WINDOWS_SEMAPHORE_HXX_
#define _XOS_MT_MICROSOFT_WINDOWS_SEMAPHORE_HXX_

#include "xos/mt/os/Os.hxx"
#include "xos/mt/Semaphore.hxx"

namespace xos {
namespace mt {
namespace microsoft {
namespace windows {

///////////////////////////////////////////////////////////////////////
///  Class: SemaphoreT
///////////////////////////////////////////////////////////////////////
template 
<typename TError = kern_return_t, TError VErrorSuccess = 0, TError VErrorFailed = -1,
 typename TAttached = HANDLE, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::SemaphoreT<TAttached, TUnattached, VUnattached>,
 class TSemaphore = extended::SemaphoreT
 <TError, VErrorSuccess, VErrorFailed, TAttached, TUnattached, VUnattached, TImplement>,
 class TImplements = TImplement, class TExtends = TSemaphore>

class _EXPORT_CLASS SemaphoreT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Extends::Error Error;
    static const Error ErrorSuccess = Extends::ErrorSuccess;
    static const Error ErrorFailed = Extends::ErrorFailed;

    typedef typename Extends::Attached Attached;
    static const typename Extends::UnattachedT Unattached = Extends::Unattached;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    SemaphoreT(Attached detached, bool isCreated, bool isLogged, bool isErrLogged): Extends(detached, isCreated, isLogged, isErrLogged) {
    }
    SemaphoreT(Attached detached, bool isCreated, bool isLogged): Extends(detached, isCreated, isLogged) {
    }
    SemaphoreT(Attached detached, bool isCreated): Extends(detached, isCreated) {
    }
    SemaphoreT(Attached detached): Extends(detached) {
    }
    SemaphoreT(bool isLogged, bool isErrLogged): Extends(isLogged, isErrLogged) {
        XOS_MT_SEMAPHORE_CREATED();
    }
    SemaphoreT(bool isLogged): Extends(isLogged) {
        XOS_MT_SEMAPHORE_CREATED();
    }
    SemaphoreT() {
        XOS_MT_SEMAPHORE_CREATED();
    }
    SemaphoreT(const SemaphoreT &copy): Extends(copy.AttachedTo(), false) {
    }
    virtual ~SemaphoreT() {
        XOS_MT_SEMAPHORE_DESTROYED();
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Attached CreateAttached(size_t initiallyReleased, size_t maximumReleased) {
        Attached detached = ((Attached)Unattached);
        if ((detached = CreateDetached(initiallyReleased, maximumReleased))) {
            this->Attach(detached);
        }
        return detached;
    }
    virtual Attached CreateAttached(size_t initiallyReleased) {
        Attached detached = ((Attached)Unattached);
        if ((detached = CreateDetached(initiallyReleased))) {
            this->Attach(detached);
        }
        return detached;
    }
    virtual Attached CreateAttached() {
        Attached detached = ((Attached)Unattached);
        if ((detached = CreateDetached())) {
            this->Attach(detached);
        }
        return detached;
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual AcquireStatus TimedAcquire(mseconds_t milliseconds) { 
        return TimedAcquireDetached(this->AttachedTo(), milliseconds); 
    }
    virtual AcquireStatus TryAcquire() { 
        return TryAcquireDetached(this->AttachedTo()); 
    }
    virtual AcquireStatus UntimedAcquire() { 
        return UntimedAcquireDetached(this->AttachedTo()); 
    }
    virtual AcquireStatus UntimedRelease() { 
        return UntimedReleaseDetached(this->AttachedTo()); 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual AcquireStatus TimedAcquireDetached(Attached detached, mseconds_t milliseconds) const { 
        if (0 > (milliseconds)) {
            return UntimedAcquireDetached(detached);
        } else {
            if (((Attached)Unattached) != detached) {
                mseconds_t millisecondsThreashold = this->TimedLoggedThreasholdMilliseconds();
                bool isLogged = ((this->IsLogged()) && (milliseconds >= millisecondsThreashold));
                DWORD dwMilliseconds = (DWORD)(milliseconds), dwResult = 0;

                IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "::WaitForSingleObject(detached, dwMilliseconds)...");
                if (WAIT_OBJECT_0 == (dwResult = ::WaitForSingleObject(detached, dwMilliseconds))) {
                    IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "...::WaitForSingleObject(detached, dwMilliseconds)");
                    return AcquireSuccess;
                } else {
                    if (WAIT_TIMEOUT == (dwResult)) {
                        IF_ERR_LOGGED_DEBUG(isLogged, isLogged, "...failed WAIT_TIMEOUT dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)");
                        return AcquireBusy;
                    } else {
                        if (WAIT_ABANDONED == (dwResult)) {
                            IS_ERR_LOGGED_ERROR("...failed WAIT_ABANDONED dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)");
                            return AcquireInterrupted;
                        } else {
                            IS_ERR_LOGGED_ERROR("...failed dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)");
                        }
                    }
                }
            }
        }
        return AcquireFailed; 
    }
    virtual AcquireStatus TryAcquireDetached(Attached detached) const { 
        mseconds_t milliseconds = 0;
        return TimedAcquireDetached(detached, milliseconds); 
    }
    virtual AcquireStatus UntimedAcquireDetached(Attached detached) const { 
        if (((Attached)Unattached) != detached) {
            DWORD dwMilliseconds = INFINITE, dwResult = 0;

            IS_ERR_LOGGED_DEBUG("::WaitForSingleObject(detached, dwMilliseconds)...");
            if (WAIT_OBJECT_0 == (dwResult = ::WaitForSingleObject(detached, dwMilliseconds))) {
                IS_ERR_LOGGED_DEBUG("...::WaitForSingleObject(detached, dwMilliseconds)");
                return AcquireSuccess;
            } else {
                if (WAIT_TIMEOUT == (dwResult)) {
                    IS_ERR_LOGGED_ERROR("...failed WAIT_TIMEOUT dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)");
                    return AcquireBusy;
                } else {
                    if (WAIT_ABANDONED == (dwResult)) {
                        IS_ERR_LOGGED_ERROR("...failed WAIT_ABANDONED dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)");
                        return AcquireInterrupted;
                    } else {
                        IS_ERR_LOGGED_ERROR("...failed dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)");
                    }
                }
            }
        }
        return AcquireFailed; 
    }
    virtual AcquireStatus UntimedReleaseDetached(Attached detached) const { 
        size_t count = 1;
        return UntimedReleaseDetached(detached, count); 
    }
    virtual AcquireStatus UntimedReleaseDetached(Attached detached, size_t count) const { 
        if (((Attached)Unattached) != detached) {
            LONG lReleaseCount = ((LONG)count), lPreviousCount = 0;

            IS_ERR_LOGGED_DEBUG("::ReleaseSemaphore(detached, lReleaseCount, &lPreviousCount)...");
            if ((::ReleaseSemaphore(detached, lReleaseCount, &lPreviousCount))) {
                IS_ERR_LOGGED_DEBUG("...::ReleaseSemaphore(detached, lReleaseCount, &lPreviousCount)");
                return ReleaseSuccess;
            } else {
                DWORD dwError = GetLastError();
                IS_ERR_LOGGED_ERROR("...failed dwError = " << dwError << " on ::ReleaseSemaphore(detached, lReleaseCount, &lPreviousCount)");
            }
        }
        return ReleaseFailed; 
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Attached CreateDetached() const {
        size_t initiallyReleased = 0;
        return CreateDetached(initiallyReleased);
    }
    virtual Attached CreateDetached(size_t initiallyReleased) const {
        size_t maximumReleased = 0;
        return CreateDetached(initiallyReleased, maximumReleased);
    }
    virtual Attached CreateDetached(size_t initiallyReleased, size_t maximumReleased) const {
        Attached detached = ((Attached)Unattached);
        LONG lInitialCount = ((LONG)initiallyReleased);
        LONG lMaximumCount = ((LONG)(maximumReleased)?(maximumReleased):(((ULONG)-1)>>1));
        LPSECURITY_ATTRIBUTES lpSecurityAttributes = 0;
        LPCTSTR lpName = 0;

        IS_ERR_LOGGED_DEBUG("::CreateSemaphore(lpSecurityAttributes, lInitialCount, lMaximumCount, lpName)...");
        if ((detached = ::CreateSemaphore(lpSecurityAttributes, lInitialCount, lMaximumCount, lpName))) {
            IS_ERR_LOGGED_DEBUG("...::CreateSemaphore(lpSecurityAttributes, lInitialCount, lMaximumCount, lpName)");
        } else {
            DWORD dwError = ::GetLastError();
            IS_ERR_LOGGED_ERROR("...failed dwError = " << dwError << " on ::CreateSemaphore(lpSecurityAttributes, lInitialCount, lMaximumCount, lpName)");
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
    
}; /// class _EXPORT_CLASS SemaphoreT
typedef SemaphoreT<> Semaphore;

} /// namespace windows
} /// namespace microsoft
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_MICROSOFT_WINDOWS_SEMAPHORE_HXX_
