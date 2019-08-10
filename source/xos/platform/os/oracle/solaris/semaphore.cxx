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
///   File: semaphore.cxx
///
/// Author: $author$
///   Date: 7/23/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/os/oracle/solaris/semaphore.hxx"
#include "xos/mt/os/Semaphore.hxx"

namespace xos {
namespace platform {
namespace os {
namespace oracle {
namespace solaris {

} /// namespace solaris
} /// namespace oracle
} /// namespace os
} /// namespace platform
} /// namespace xos

#if !defined(SOLARIS)
///
/// solaris semaphores
/// ...
int sema_init(sema_t *sp, unsigned int count, int type, void * arg) {
    ::xos::mt::os::Semaphore** ppSemaphore = 0;

    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)sp))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;

        if ((pSemaphore = new ::xos::mt::os::Semaphore(((::xos::mt::os::Semaphore::Attached)::xos::mt::os::Semaphore::Unattached), false, false))) {
            if ((pSemaphore->Create(count))) {
                *ppSemaphore = pSemaphore;
                return 0;
            }
            delete pSemaphore;
        }
    }
    return EINVAL;
}   
int sema_destroy(sema_t *sp) {
    ::xos::mt::os::Semaphore** ppSemaphore = 0;

    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)sp))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;

        if ((pSemaphore = (*ppSemaphore))) {
            delete pSemaphore;
            *ppSemaphore = 0;
            return 0;
        }
    }
    return EINVAL;
}   
int sema_post(sema_t *sp) {
    ::xos::mt::os::Semaphore** ppSemaphore = 0;

    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)sp))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;

        if ((pSemaphore = (*ppSemaphore))) {
            if ((pSemaphore->Release())) {
                return 0;
            }
        }
    }
    return EINVAL;
}   
int sema_wait(sema_t *sp) {
    ::xos::mt::os::Semaphore** ppSemaphore = 0;

    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)sp))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;

        if ((pSemaphore = (*ppSemaphore))) {
            if ((pSemaphore->Acquire())) {
                return 0;
            }
        }
    }
    return EINVAL;
}   
int sema_trywait(sema_t *sp) {
    ::xos::mt::os::Semaphore** ppSemaphore = 0;

    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)sp))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;

        if ((pSemaphore = (*ppSemaphore))) {
            ::xos::AcquireStatus status = ::xos::AcquireFailed;
            if (::xos::AcquireSuccess == (status = pSemaphore->TryAcquire())) {
                return 0;
            } else {
                if (::xos::AcquireBusy == (status)) {
                    return EBUSY;
                } else {
                    if (::xos::AcquireInterrupted == (status)) {
                        return EINTR;
                    } else {
                    }
                }
            }
        }
    }
    return EINVAL;
}   
int sema_timedwait(sema_t *sp, timestruc_t *abstime) {
    ::xos::mt::os::Semaphore** ppSemaphore = 0;

    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)sp))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;

        if ((pSemaphore = (*ppSemaphore)) && (abstime)) {
            int err = 0;
            timestruc_t reltime;
            if (!(err = ::clock_gettime(CLOCK_REALTIME, &reltime))) {
                reltime.tv_sec = abstime->tv_sec - reltime.tv_sec;
                reltime.tv_nsec = abstime->tv_nsec - reltime.tv_nsec;
                err = sema_reltimedwait(sp, &reltime);
                return err;
            }
        }
    }
    return EINVAL;
}   
int sema_reltimedwait(sema_t *sp, timestruc_t *reltime) {
    ::xos::mt::os::Semaphore** ppSemaphore = 0;

    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)sp))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;

        if ((pSemaphore = (*ppSemaphore)) && (reltime)) {
            mseconds_t milliseconds = ::xos::SecondsMSeconds(reltime->tv_sec) + ::xos::NSecondsMSeconds(reltime->tv_nsec);
            ::xos::AcquireStatus status = ::xos::AcquireFailed;

            if (::xos::AcquireSuccess == (status = pSemaphore->TimedAcquire(milliseconds))) {
                return 0;
            } else {
                if (::xos::AcquireBusy == (status)) {
                    return ETIME;
                } else {
                    if (::xos::AcquireInterrupted == (status)) {
                        return EINTR;
                    } else {
                    }
                }
            }
        }
    }
    return EINVAL;
}   
/// ...
/// solaris semaphores
/// 
#endif /// !defined(SOLARIS)
