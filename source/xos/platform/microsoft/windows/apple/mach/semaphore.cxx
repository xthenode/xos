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
///   Date: 5/27/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/microsoft/windows/apple/mach/semaphore.hxx"

namespace xos {
namespace platform {
namespace microsoft {
namespace windows {
namespace apple {
namespace mach {

} /// namespace mach
} /// namespace apple
} /// namespace windows
} /// namespace microsoft
} /// namespace platform
} /// namespace xos

kern_return_t semaphore_create (task_t task, semaphore_t *semaphore, int policy, int value) {
    kern_return_t err = KERN_FAILURE;
    if ((semaphore)) {
            ::xos::mt::os::Semaphore* sem = 0;
            if ((sem = new ::xos::mt::os::Semaphore(NULL, false, false))) {
                bool success = (value)?(sem->Create(value)):(sem->Create());
                if ((success)) {
                    *semaphore = sem;
                    err = KERN_SUCCESS;
                } else {
                    delete sem;
                }
            }
    }
    return err;
}
kern_return_t semaphore_destroy (task_t task, semaphore_t semaphore) {
    kern_return_t err = KERN_FAILURE;
    if ((semaphore)) {
        ::xos::mt::os::Semaphore* sem = 0;
        if ((sem = ((::xos::mt::os::Semaphore*)(semaphore)))) {
            if ((sem->Destroy())) {
                err = KERN_SUCCESS;
            }
            delete sem;
        }
    }
    return err;
}

kern_return_t semaphore_signal(semaphore_t semaphore) {
    kern_return_t err = KERN_FAILURE;
    if ((semaphore)) {
        ::xos::mt::os::Semaphore* sem = 0;
        if ((sem = ((::xos::mt::os::Semaphore*)(semaphore)))) {
            if ((sem->Release())) {
                err = KERN_SUCCESS;
            }
        }
    }
    return err;
}
kern_return_t semaphore_wait(semaphore_t semaphore) {
    kern_return_t err = KERN_FAILURE;
    if ((semaphore)) {
        ::xos::mt::os::Semaphore* sem = 0;
        if ((sem = ((::xos::mt::os::Semaphore*)(semaphore)))) {
            if ((sem->Acquire())) {
                err = KERN_SUCCESS;
            }
        }
    }
    return err;
}
kern_return_t semaphore_timedwait(semaphore_t semaphore, mach_timespec_t wait_time) {
    kern_return_t err = KERN_FAILURE;
    if ((semaphore)) {
        ::xos::mt::os::Semaphore* sem = 0;
        if ((sem = ((::xos::mt::os::Semaphore*)(semaphore)))) {
            ::xos::AcquireStatus status = ::xos::AcquireFailed;
            mseconds_t milliseconds = ::xos::SecondsMSeconds(wait_time.tv_sec) + ::xos::NSecondsMSeconds(wait_time.tv_nsec);
            if (::xos::AcquireSuccess == (status = sem->TimedAcquire(milliseconds))) {
                err = KERN_SUCCESS;
            }
        }
    }
    return err;
}
