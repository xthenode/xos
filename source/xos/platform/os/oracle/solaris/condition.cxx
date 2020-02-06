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
///   File: condition.cxx
///
/// Author: $author$
///   Date: 7/29/2019
///////////////////////////////////////////////////////////////////////
#include "xos/platform/os/oracle/solaris/condition.hxx"
#include "xos/mt/oracle/solaris/Condition.hxx"
#include "xos/mt/os/Condition.hxx"

namespace xos {
namespace mt {
namespace oracle {
namespace solaris {

class _EXPORTED_ ConditionFriend {
public:
    ConditionFriend(const Condition& condition): _condition(condition) {
    }
    virtual bool* Signaled() const {
        return (bool*)_condition._signaled;
    }
    virtual mutex_t* Mutex() const {
        return (mutex_t*)_condition._mutex;
    }
protected:
    const Condition& _condition;
};

} /// namespace solaris
} /// namespace oracle
} /// namespace mt
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
/// solaris conditions
/// ...
int cond_init(cond_t *cvp, int type, void *arg) { 
    ::xos::mt::os::Condition** ppCondition = 0;

    if ((ppCondition = ((::xos::mt::os::Condition**)cvp))) {
        ::xos::mt::oracle::solaris::Condition* pCondition = 0;
        
        if ((pCondition = ((::xos::mt::oracle::solaris::Condition*)arg))) {
            ::xos::mt::oracle::solaris::ConditionFriend condition(*pCondition);
            ::xos::mt::oracle::solaris::Condition::MutexAttached conditionMutex = 0;
            bool* signaled = 0; 
    
            if ((signaled = condition.Signaled()) && (conditionMutex = condition.Mutex())) {
                ::xos::mt::os::Mutex** ppMutex = ((::xos::mt::os::Mutex**)conditionMutex);
                ::xos::mt::os::Mutex* pMutex = *ppMutex;
        
                if ((pMutex)) {
                    ::xos::mt::os::Condition::MutexAttached mutex = pMutex->AttachedTo();
                    
                    if ((mutex)) {
                        ::xos::mt::os::Condition* pCondition = 0;

                        if ((pCondition = new ::xos::mt::os::Condition(((::xos::mt::os::Condition::Attached)::xos::mt::os::Condition::Unattached), false, false, mutex, *signaled))) {
                            if ((pCondition->Create())) {
                                *ppCondition = pCondition;
                                return 0;
                            }
                            delete pCondition;
                        }
                    }
                }
            }
        }
    }
    return EINVAL; 
} 
int cond_destroy(cond_t *cvp) { 
    ::xos::mt::os::Condition** ppCondition = 0;

    if ((ppCondition = ((::xos::mt::os::Condition**)cvp))) {
        ::xos::mt::os::Condition* pCondition = 0;

        if ((pCondition = (*ppCondition))) {
            delete pCondition;
            *ppCondition = 0;
            return 0;
        }
    }
    return EINVAL; 
}
int cond_wait(cond_t *cvp, mutex_t *mp) { 
    ::xos::mt::os::Condition** ppCondition = 0;

    if ((ppCondition = ((::xos::mt::os::Condition**)cvp))) {
        ::xos::mt::os::Condition* pCondition = 0;

        if ((pCondition = (*ppCondition))) {
            ::xos::mt::os::Mutex** ppMutex = 0;
        
            if ((ppMutex = ((::xos::mt::os::Mutex**)mp))) {
                ::xos::mt::os::Mutex* pMutex = 0;
        
                if ((pMutex = (*ppMutex))) {
                    ::xos::mt::os::Mutex::Attached mutex = 0;
                    
                    if ((mutex = pMutex->AttachedTo())) {
                        
                        if ((pCondition->Wait(*mutex))) {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return EINVAL; 
}
int cond_timedwait(cond_t *cvp, mutex_t *mp, timestruc_t *abstime) {
    if ((abstime)) {
        int err = 0;
        timestruc_t reltime;
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &reltime))) {
            reltime.tv_sec = abstime->tv_sec - reltime.tv_sec;
            reltime.tv_nsec = abstime->tv_nsec - reltime.tv_nsec;
            return cond_reltimedwait(cvp, mp, &reltime);
        }
    }
    return EINVAL; 
}
int cond_reltimedwait(cond_t *cvp, mutex_t *mp, timestruc_t *reltime) { 
    ::xos::mt::os::Condition** ppCondition = 0;

    if ((ppCondition = ((::xos::mt::os::Condition**)cvp))) {
        ::xos::mt::os::Condition* pCondition = 0;

        if ((pCondition = (*ppCondition))) {
            ::xos::mt::os::Mutex** ppMutex = 0;
        
            if ((ppMutex = ((::xos::mt::os::Mutex**)mp))) {
                ::xos::mt::os::Mutex* pMutex = 0;
        
                if ((pMutex = (*ppMutex))) {
                    ::xos::mt::os::Mutex::Attached mutex = 0;
                    
                    if ((mutex = pMutex->AttachedTo())) {
                        if ((reltime)) {
                            mseconds_t milliseconds = ::xos::SecondsMSeconds(reltime->tv_sec) + ::xos::NSecondsMSeconds(reltime->tv_nsec);
                            ::xos::WaitStatus status = pCondition->TimedWait(*mutex, milliseconds);
                            
                            if (::xos::WaitSuccess == (status)) {
                                return 0;
                            } else {
                                if (::xos::WaitBusy == (status)) {
                                    return ETIME;
                                } else {
                                    if (::xos::WaitInterrupted == (status)) {
                                        return EINTR;
                                    } else {
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return EINVAL; 
}
int cond_signal(cond_t *cvp) { 
    ::xos::mt::os::Condition** ppCondition = 0;

    if ((ppCondition = ((::xos::mt::os::Condition**)cvp))) {
        ::xos::mt::os::Condition* pCondition = 0;

        if ((pCondition = (*ppCondition))) {
            ::xos::mt::os::Condition::Attached detached = ((::xos::mt::os::Condition::Attached)::xos::mt::os::Condition::Unattached);

            if (((::xos::mt::os::Condition::Attached)::xos::mt::os::Condition::Unattached) != (detached = pCondition->AttachedTo())) {
                if ((pCondition->SignalDetached(*detached))) {
                    return 0;
                }
            }
        }
    }
    return EINVAL; 
}
int cond_broadcast(cond_t *cvp) { 
    ::xos::mt::os::Condition** ppCondition = 0;

    if ((ppCondition = ((::xos::mt::os::Condition**)cvp))) {
        ::xos::mt::os::Condition* pCondition = 0;

        if ((pCondition = (*ppCondition))) {
            ::xos::mt::os::Condition::Attached detached = ((::xos::mt::os::Condition::Attached)::xos::mt::os::Condition::Unattached);

            if (((::xos::mt::os::Condition::Attached)::xos::mt::os::Condition::Unattached) != (detached = pCondition->AttachedTo())) {
                if ((pCondition->BroadcastDetached(*detached))) {
                    return 0;
                }
            }
        }
    }
    return EINVAL; 
}
/// ...
/// solaris conditions
///
#endif /// !defined(SOLARIS)
