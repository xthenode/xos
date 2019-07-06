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
///   Date: 5/9/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_MICROSOFT_WINDOWS_SEMAPHORE_HXX_
#define _XOS_MT_MICROSOFT_WINDOWS_SEMAPHORE_HXX_

#include "xos/mt/os/Os.hxx"
#include "xos/mt/Semaphore.hxx"

namespace xos {
namespace mt {
namespace microsoft {
namespace windows {

typedef int SemaphoreTError;
static const SemaphoreTError SemaphoreVErrorSuccess = 0;
typedef HANDLE SemaphoreTAttached;
typedef int SemaphoreTUnattached;
static const SemaphoreTUnattached SemaphoreVUnattached = 0;
///////////////////////////////////////////////////////////////////////
///  Class: SemaphoreT
///////////////////////////////////////////////////////////////////////
template 
<class TImplement = implemented::SemaphoreT
 <SemaphoreTAttached, SemaphoreTUnattached, SemaphoreVUnattached>,
 class TSemaphore = extended::SemaphoreT
 <SemaphoreTError, SemaphoreVErrorSuccess, 
  SemaphoreTAttached, SemaphoreTUnattached, SemaphoreVUnattached, TImplement>,
 class TImplements = TImplement, class TExtends = TSemaphore>

class _EXPORT_CLASS SemaphoreT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Extends::Attached Attached;
    static const typename Extends::UnattachedT Unattached = Extends::Unattached;
    
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
        ///XOS_MT_SEMAPHORE_CREATED();
        if (!(this->Created())) { \
            const CreateException e(CreateFailed); \
            IS_ERR_LOGGED_ERROR("...failed on this->Created() throw (const CreateException e = CreateFailed)..."); \
            throw (e); \
        }
    }
    SemaphoreT(const SemaphoreT &copy): Extends(copy.AttachedTo(), false) {
    }
    virtual ~SemaphoreT() {
        XOS_MT_SEMAPHORE_DESTROYED();
    }

    using Extends::CreateAttached;
    virtual Attached CreateAttached(size_t initiallyReleased, size_t maximumReleased) {
        Attached detached = ((Attached)Unattached);
        if ((detached = CreateDetached(initiallyReleased, maximumReleased))) {
            this->Attach(detached);
        }
        return detached;
    }
    using Extends::CreateDetached;
    virtual Attached CreateDetached(size_t initiallyReleased) const {
        Attached detached = CreateDetached(initiallyReleased, 0);
        return detached;
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

    virtual AcquireStatus TimedAcquire(mseconds_t milliseconds) { 
        if (0 > (milliseconds)) {
            return UntimedAcquire();
        } else {
            Attached detached = ((Attached)Unattached);
    
            if ((detached = this->AttachedTo())) {
                DWORD dwMilliseconds = (DWORD)(milliseconds), dwResult = 0;

                IS_ERR_LOGGED_TRACE("::WaitForSingleObject(detached, dwMilliseconds)...");
                if (WAIT_OBJECT_0 == (dwResult = ::WaitForSingleObject(detached, dwMilliseconds))) {
                    IS_ERR_LOGGED_TRACE("...::WaitForSingleObject(detached, dwMilliseconds)");
                    return AcquireSuccess;
                } else {
                    if (WAIT_TIMEOUT == (dwResult)) {
                        IS_ERR_LOGGED_TRACE("...failed WAIT_TIMEOUT dwResult = " << dwResult << " on ::WaitForSingleObject(detached, dwMilliseconds)");
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
    virtual AcquireStatus UntimedAcquire() { 
        Attached detached = ((Attached)Unattached);

        if ((detached = this->AttachedTo())) {
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
    virtual AcquireStatus UntimedRelease() { 
        return UntimedRelease(1); 
    }
    virtual AcquireStatus UntimedRelease(size_t count) { 
        Attached detached = ((Attached)Unattached);

        if ((detached = this->AttachedTo())) {
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
}; /// class _EXPORT_CLASS SemaphoreT
typedef SemaphoreT<> Semaphore;

} /// namespace windows
} /// namespace microsoft
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_MICROSOFT_WINDOWS_SEMAPHORE_HXX_
