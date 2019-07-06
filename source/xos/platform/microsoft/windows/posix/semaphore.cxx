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
///   Date: 5/25/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/microsoft/windows/posix/semaphore.hxx"
#include "xos/mt/os/Semaphore.hxx"

namespace xos {
namespace platform {
namespace microsoft {
namespace windows {
namespace posix {

} /// namespace posix
} /// namespace windows
} /// namespace microsoft
} /// namespace platform
} /// namespace xos

int sem_init(sem_t *sem, int pshared, unsigned int value) { 
    int err = EFAULT;
    if ((sem)) {
        if ((!(pshared))) {
            ::xos::mt::os::Semaphore* semaphore = 0;
            if ((semaphore = new ::xos::mt::os::Semaphore(NULL, false, false))) {
                bool success = (value)?(semaphore->Create(value)):(semaphore->Create());
                if ((success)) {
                    *sem = semaphore;
                    err = 0;
                } else {
                    delete semaphore;
                }
            }
        }
    }
    return err;
}
int sem_destroy(sem_t *sem) { 
    int err = EFAULT;
    if ((sem)) {
        ::xos::mt::os::Semaphore* semaphore = 0;
        if ((semaphore = ((::xos::mt::os::Semaphore*)(*sem)))) {
            if ((semaphore->Destroy())) {
                err = 0;
            }
            *sem = 0;
            delete semaphore;
        }
    }
    return err;
}

int sem_post(sem_t *sem) { 
    int err = EFAULT;
    if ((sem)) {
        ::xos::mt::os::Semaphore* semaphore = 0;
        if ((semaphore = ((::xos::mt::os::Semaphore*)(*sem)))) {
            if ((semaphore->Release())) {
                err = 0;
            }
        }
    }
    return err;
}
int sem_wait(sem_t *sem) { 
    int err = EFAULT;
    if ((sem)) {
        ::xos::mt::os::Semaphore* semaphore = 0;
        if ((semaphore = ((::xos::mt::os::Semaphore*)(*sem)))) {
            if ((semaphore->Acquire())) {
                err = 0;
            }
        }
    }
    return err;
}
int sem_trywait(sem_t *sem) { 
    int err = EFAULT;
    return err;
}
int sem_timedwait(sem_t *sem, const struct timespec* timespec) { 
    int err = EFAULT;
    return err;
}

int sem_getvalue(sem_t *sem, int *sval) { 
    int err = EFAULT;
    return err;
}
