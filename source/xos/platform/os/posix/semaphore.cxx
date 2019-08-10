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
///   Date: 6/23/2019
/// 
/// posix semaphores to os semaphores 
///////////////////////////////////////////////////////////////////////
#include "xos/platform/os/posix/semaphore.hxx"
#include "xos/mt/os/Semaphore.hxx"

#if !defined(WINDOWS)
#if !defined(HAS_POSIX_SEMAPHORE)
#if defined(MACOSX)
#if (__MAC_OS_X_VERSION_MAX_ALLOWED <= __MAC_OS_X_VERSION_MAX_ALLOWED_NO_POSIX_SEM) 
#else /// (__MAC_OS_X_VERSION_MAX_ALLOWED <= __MAC_OS_X_VERSION_MAX_ALLOWED_NO_POSIX_SEM) 
#define HAS_POSIX_SEMAPHORE
#endif /// (__MAC_OS_X_VERSION_MAX_ALLOWED <= __MAC_OS_X_VERSION_MAX_ALLOWED_NO_POSIX_SEM) 
#else /// defined(MACOSX) 
#define HAS_POSIX_SEMAPHORE
#endif /// defined(MACOSX) 
#endif /// !defined(HAS_POSIX_SEMAPHORE)
#endif /// !defined(WINDOWS)

namespace xos {
namespace platform {
namespace os {
namespace posix {

} /// namespace posix
} /// namespace os
} /// namespace platform
} /// namespace xos

#if !defined(HAS_POSIX_SEMAPHORE)
///
/// posix semaphores
/// ...
int sem_init(sem_t *sem, int pshared, unsigned int value) { 
    ::xos::mt::os::Semaphore** ppSemaphore = 0;

    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)sem))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;

        if ((pSemaphore = new ::xos::mt::os::Semaphore(((::xos::mt::os::Semaphore::Attached)::xos::mt::os::Semaphore::Unattached), false, false))) {
            if ((pSemaphore->Create(value))) {
                *ppSemaphore = pSemaphore;
                return 0;
            }
            delete pSemaphore;
        }
    }
    return errno = EINVAL; 
}
int sem_destroy(sem_t *sem) { 
    ::xos::mt::os::Semaphore** ppSemaphore = 0;

    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)sem))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;

        if ((pSemaphore = (*ppSemaphore))) {
            delete pSemaphore;
            *ppSemaphore = 0;
            return 0;
        }
    }
    return errno = EINVAL; 
}
int sem_post(sem_t *sem) { 
    ::xos::mt::os::Semaphore** ppSemaphore = 0;

    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)sem))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;

        if ((pSemaphore = (*ppSemaphore))) {
            if ((pSemaphore->Release())) {
                return 0;
            }
        }
    }
    return errno = EINVAL; 
}
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout) {
    return errno = EINVAL; 
}
int sem_timedwait_relative_np(sem_t *sem, const struct timespec *timeout) {
    ::xos::mt::os::Semaphore** ppSemaphore = 0;

    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)sem))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;

        if ((pSemaphore = (*ppSemaphore))) {
            if (0 < (timeout)) {
                ::xos::AcquireStatus status = ::xos::AcquireFailed;
                mseconds_t milliseconds = 0;

                milliseconds += ::xos::SecondsMSeconds(timeout->tv_sec);
                milliseconds += ::xos::NSecondsMSeconds(timeout->tv_nsec);

                if (::xos::AcquireSuccess == (status = pSemaphore->TimedAcquire(milliseconds))) {
                    return 0;
                } else {
                    if (::xos::AcquireBusy == (status)) {
                        return errno = ETIMEDOUT;
                    } else {
                        if (::xos::AcquireInterrupted == (status)) {
                            return errno = EINTR;
                        } else {
                        }
                    }
                }
            } else {
                if (0 > (timeout)) {
                    return sem_wait(sem);
                } else {
                    return sem_trywait(sem);
                }
            }
        }
    }
    return errno = EINVAL; 
}
int sem_trywait(sem_t *sem) { 
    ::xos::mt::os::Semaphore** ppSemaphore = 0;

    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)sem))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;

        if ((pSemaphore = (*ppSemaphore))) {
            ::xos::AcquireStatus status = ::xos::AcquireFailed;
            if (::xos::AcquireSuccess == (status = pSemaphore->TryAcquire())) {
                return 0;
            } else {
                if (::xos::AcquireBusy == (status)) {
                    return errno = EAGAIN;
                } else {
                    if (::xos::AcquireInterrupted == (status)) {
                        return errno = EINTR;
                    } else {
                    }
                }
            }
        }
    }
    return errno = EINVAL; 
}
int sem_wait(sem_t *sem) { 
    ::xos::mt::os::Semaphore** ppSemaphore = 0;

    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)sem))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;

        if ((pSemaphore = (*ppSemaphore))) {
            if ((pSemaphore->Acquire())) {
                return 0;
            }
        }
    }
    return errno = EINVAL; 
}
/// ...
/// posix semaphores
///
#endif /// !defined(HAS_POSIX_SEMAPHORE)
