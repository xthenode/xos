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
///   File: mutex.cxx
///
/// Author: $author$
///   Date: 5/24/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/microsoft/windows/posix/pthread/mutex.hxx"
#include "xos/mt/os/Mutex.hxx"

namespace xos {
namespace platform {
namespace microsoft {
namespace windows {
namespace posix {
namespace pthread {

} /// namespace pthread
} /// namespace posix
} /// namespace windows
} /// namespace microsoft
} /// namespace platform
} /// namespace xos

///
/// pthread_mutexattr_t
///
int pthread_mutexattr_init(pthread_mutexattr_t* mutexattr) {
    int err = EINVAL;
    if ((mutexattr)) {
        *mutexattr = NULL;
        err = 0;
    }
    return err;
}
int pthread_mutexattr_destroy(pthread_mutexattr_t* mutexattr) {
    int err = EINVAL;
    if ((mutexattr) && (!(*mutexattr))) {
        err = 0;
    }
    return err;
}

int pthread_mutexattr_settype(pthread_mutexattr_t* mutexattr, int type) {
    int err = EINVAL;
    return err;
}
int pthread_mutexattr_gettype(const pthread_mutexattr_t* mutexattr, int* type) {
    int err = EINVAL;
    return err;
}

int pthread_mutexattr_setpshared(pthread_mutexattr_t* mutexattr, int type) {
    int err = EINVAL;
    return err;
}
int pthread_mutexattr_getpshared(const pthread_mutexattr_t* mutexattr, int* type) {
    int err = EINVAL;
    return err;
}

///
/// pthread_mutex_t
///
int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* mutexattr) {
    int err = EFAULT;
    if ((mutexattr) && (!(*mutexattr)) && (mutex)) {
        ::xos::mt::os::Mutex* mtx = 0;
        if ((mtx = new ::xos::mt::os::Mutex(NULL, false, false))) {
            if ((mtx->Create())) {
                *mutex = mtx;
                err = 0;
            } else {
                delete mtx;
            }
        }
    }
    return err;
}
int pthread_mutex_destroy(pthread_mutex_t* mutex) {
    int err = EFAULT;
    if ((mutex)) {
        ::xos::mt::os::Mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::Mutex*)(*mutex)))) {
            if ((mtx->Destroy())) {
                err = 0;
            }
            *mutex = 0;
            delete mtx;
        }
    }
    return err;
}

int pthread_mutex_lock(pthread_mutex_t* mutex) {
    int err = EFAULT;
    if ((mutex)) {
        ::xos::mt::os::Mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::Mutex*)(*mutex)))) {
            if ((mtx->Lock())) {
                err = 0;
            }
        }
    }
    return err;
}
int pthread_mutex_unlock(pthread_mutex_t* mutex) {
    int err = EFAULT;
    if ((mutex)) {
        ::xos::mt::os::Mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::Mutex*)(*mutex)))) {
            if ((mtx->Unlock())) {
                err = 0;
            }
        }
    }
    return err;
}

int pthread_mutex_trylock(pthread_mutex_t* mutex) {
    int err = EFAULT;
    if ((mutex)) {
        ::xos::mt::os::Mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::Mutex*)(*mutex)))) {
            if (::xos::LockSuccess == (mtx->TryLock())) {
                err = 0;
            }
        }
    }
    return err;
}
int pthread_mutex_timedlock(pthread_mutex_t* mutex, const struct timespec* timespec) {
    int err = EFAULT;
    if ((mutex) && (timespec)) {
        ::xos::mt::os::Mutex* mtx = 0;
        if ((mtx = ((::xos::mt::os::Mutex*)(*mutex)))) {
            mseconds_t milliSeconds = ::xos::SecondsMSeconds(timespec->tv_sec)+::xos::NSecondsMSeconds(timespec->tv_nsec);
            if (::xos::LockSuccess == (mtx->TimedLock(milliSeconds))) {
                err = 0;
            }
        }
    }
    return err;
}
