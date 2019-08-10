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
///   Date: 7/14/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/os/apple/mach/semaphore.hxx"

namespace xos {
namespace platform {
namespace os {
namespace apple {
namespace mach {

} /// namespace mach
} /// namespace apple
} /// namespace os
} /// namespace platform
} /// namespace xos

#if !defined(APPLEOS)
///
/// mach semaphores
/// ...
kern_return_t semaphore_create(task_t task, semaphore_t *semaphore, int policy, int value) {
    ::xos::mt::os::Semaphore** ppSemaphore = 0;
    if ((ppSemaphore = ((::xos::mt::os::Semaphore**)semaphore))) {
        ::xos::mt::os::Semaphore* pSemaphore = 0;
        if ((pSemaphore = new ::xos::mt::os::Semaphore(((::xos::mt::os::Semaphore::Attached)::xos::mt::os::Semaphore::Unattached)))) {
            if ((pSemaphore->Create(value))) {
                *ppSemaphore = pSemaphore;
                return KERN_SUCCESS;
            }
            delete pSemaphore;
        }
    }
    return KERN_FAILURE; 
}
kern_return_t semaphore_destroy(task_t task, semaphore_t semaphore) {
    ::xos::mt::os::Semaphore* pSemaphore = 0;
    if ((pSemaphore = ((::xos::mt::os::Semaphore*)semaphore))) {
        delete pSemaphore;
        pSemaphore = 0;
        return KERN_SUCCESS;
    }
    return KERN_FAILURE; 
}
kern_return_t semaphore_signal(semaphore_t semaphore ) { 
    ::xos::mt::os::Semaphore* pSemaphore = 0;
    if ((pSemaphore = ((::xos::mt::os::Semaphore*)semaphore))) {
        if ((pSemaphore->Release())) {
            return KERN_SUCCESS;
        }
    }
    return KERN_FAILURE; 
}
kern_return_t semaphore_signal_all(semaphore_t semaphore ) { 
    return KERN_FAILURE; 
}
kern_return_t semaphore_wait(semaphore_t semaphore ) { 
    ::xos::mt::os::Semaphore* pSemaphore = 0;
    if ((pSemaphore = ((::xos::mt::os::Semaphore*)semaphore))) {
        if ((pSemaphore->Acquire())) {
            return KERN_SUCCESS;
        }
    }
    return KERN_FAILURE; 
}
kern_return_t semaphore_trywait(semaphore_t semaphore ) { 
    ::xos::mt::os::Semaphore* pSemaphore = 0;
    if ((pSemaphore = ((::xos::mt::os::Semaphore*)semaphore))) {
        ::xos::AcquireStatus status = ::xos::AcquireFailed;
        if (::xos::AcquireSuccess == (status = pSemaphore->TryAcquire())) {
            return KERN_SUCCESS;
        } else {
            if (::xos::AcquireBusy == (status)) {
                return KERN_OPERATION_TIMED_OUT;
            } else {
                if (::xos::AcquireInterrupted == (status)) {
                    return KERN_ABORTED;
                } else {
                }
            }
        }
    }
    return KERN_FAILURE; 
}
kern_return_t semaphore_timedwait(semaphore_t semaphore, mach_timespec_t wait_time ) { 
    ::xos::mt::os::Semaphore* pSemaphore = 0;
    if ((pSemaphore = ((::xos::mt::os::Semaphore*)semaphore))) {
        ::xos::AcquireStatus status = ::xos::AcquireFailed;
        mseconds_t milliseconds = 0;
        milliseconds += ::xos::SecondsMSeconds(wait_time.tv_sec);
        milliseconds += ::xos::NSecondsMSeconds(wait_time.tv_nsec);
        if (::xos::AcquireSuccess == (status = pSemaphore->TimedAcquire(milliseconds))) {
            return KERN_SUCCESS;
        } else {
            if (::xos::AcquireBusy == (status)) {
                return KERN_OPERATION_TIMED_OUT;
            } else {
                if (::xos::AcquireInterrupted == (status)) {
                    return KERN_ABORTED;
                } else {
                }
            }
        }
    }
    return KERN_FAILURE; 
}
kern_return_t semaphore_timedwait_signal(semaphore_t wait_semaphore, semaphore_t signal_semaphore, mach_timespec_t wait_time ) { 
    return KERN_FAILURE; 
}
kern_return_t semaphore_wait_signal(semaphore_t wait_semaphore, semaphore_t signal_semaphore ) { 
    return KERN_FAILURE; 
}
kern_return_t semaphore_signal_thread(semaphore_t semaphore, thread_t thread ) { 
    return KERN_FAILURE; 
}
/// ...
/// mach semaphores
///
#endif /// !defined(APPLEOS)
