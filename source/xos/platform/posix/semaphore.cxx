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
///   Date: 5/11/2019
///////////////////////////////////////////////////////////////////////
#include "xos/mt/os/Semaphore.hxx"
#include "xos/platform/posix/semaphore.hxx"

#if !defined(HAS_POSIX_SEMAPHORE)
#if !defined(WINDOWS)
#if defined(MACOSX)
#if (__MAC_OS_X_VERSION_MAX_ALLOWED <= __MAC_OS_X_VERSION_MAX_ALLOWED_NO_POSIX_SEM) 
#else /// (__MAC_OS_X_VERSION_MAX_ALLOWED <= __MAC_OS_X_VERSION_MAX_ALLOWED_NO_POSIX_SEM) 
#define HAS_POSIX_SEMAPHORE
#endif /// (__MAC_OS_X_VERSION_MAX_ALLOWED <= __MAC_OS_X_VERSION_MAX_ALLOWED_NO_POSIX_SEM) 
#else /// defined(MACOSX) 
#define HAS_POSIX_SEMAPHORE
#endif /// defined(MACOSX) 
#endif /// !defined(WINDOWS)
#endif /// !defined(HAS_POSIX_SEMAPHORE)

namespace xos {
namespace platform {
namespace posix {

} /// namespace posix
} /// namespace platform
} /// namespace xos

#if !defined(HAS_POSIX_SEMAPHORE)
///
/// posix semaphores
/// ...
int sem_close(sem_t *sem) { 
    return EINVAL; 
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
    return EINVAL; 
}
int sem_getvalue(sem_t *sem, int *sval) { 
    return EINVAL; 
}
int sem_init(sem_t *sem, int pshared, unsigned int value) { 
    ::xos::mt::os::Semaphore** ppSemaphore = 0;

    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)sem))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;

        if ((pSemaphore = new ::xos::mt::os::Semaphore(((::xos::mt::os::Semaphore::Attached)::xos::mt::os::Semaphore::Unattached)))) {
            if ((pSemaphore->Create(value))) {
                *ppSemaphore = pSemaphore;
                return 0;
            }
            delete pSemaphore;
        }
    }
    return EINVAL; 
}
sem_t *sem_open(const char *name, int oflag, ...) { 
    return 0; 
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
    return EINVAL; 
}
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout) {
    if ((sem) && (abs_timeout)) {
        struct timespec timeout = *abs_timeout;
        return sem_timedwait_relative_np(sem, &timeout);
    }
    return EINVAL; 
}
int sem_timedwait_relative_np(sem_t *sem, const struct timespec *timeout) {
    ::xos::mt::os::Semaphore** ppSemaphore = 0;

    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)sem))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;

        if ((pSemaphore = (*ppSemaphore))) {
            if ((timeout)) {
                ::xos::AcquireStatus status = ::xos::AcquireFailed;
                mseconds_t milliseconds = 0;

                milliseconds += ::xos::SecondsMSeconds(timeout->tv_sec);
                milliseconds += ::xos::NSecondsMSeconds(timeout->tv_nsec);
                if (::xos::AcquireSuccess == (status = pSemaphore->TimedAcquire(milliseconds))) {
                    return 0;
                } else {
                    if (::xos::AcquireBusy == (status)) {
                        return ETIMEDOUT;
                    } else {
                        if (::xos::AcquireInterrupted == (status)) {
                            return EINTR;
                        } else {
                        }
                    }
                }
            }
        }
    }
    return EINVAL; 
}
int sem_trywait(sem_t *sem) { 
    return EINVAL; 
}
int sem_unlink(const char *name) { 
    return EINVAL; 
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
    return EINVAL; 
}
int semctl(int semid, int semnum, int cmd, ...) { 
    return EINVAL; 
}
int semget(key_t key, int nsems, int semflg) { 
    return EINVAL; 
}
int semop(int semid, struct sembuf *sops, size_t nsops) { 
    return EINVAL; 
}
/// ...
/// posix semaphores
///
#endif /// !defined(HAS_POSIX_SEMAPHORE)
