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
///   File: Thread.hxx
///
/// Author: $author$
///   Date: 7/7/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_POSIX_THREAD_HXX_
#define _XOS_MT_POSIX_THREAD_HXX_

#include "xos/mt/Thread.hxx"

#include <pthread.h>
#include <time.h>
#include <errno.h>

#if defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#if !defined(PTHREAD_HAS_TRYJOIN)
#define PTHREAD_HAS_TRYJOIN
#endif // !defined(PTHREAD_HAS_TRYJOIN)
#if !defined(PTHREAD_HAS_TIMEDJOIN)
#define PTHREAD_HAS_TIMEDJOIN
#endif // !defined(PTHREAD_HAS_TIMEDJOIN)
#else // defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )
#if !defined(CLOCK_REALTIME)
#define CLOCK_REALTIME 0
#define clockid_t int
inline int clock_gettime(clockid_t clk_id, struct timespec *res) {
    if ((res)) {
        memset(res, 0, sizeof(struct timespec));
        return 0;
    }
    return EINVAL;
}
#endif /// !defined(CLOCK_REALTIME)
#if !defined(PTHREAD_HAS_TRYJOIN)
#define PTHREAD_HAS_TRYJOIN
inline int pthread_tryjoin_np(pthread_t thread, void **retval) {
    return EINVAL;
}
#endif // !defined(PTHREAD_HAS_TRYJOIN)
#if !defined(PTHREAD_HAS_TIMEDJOIN)
#define PTHREAD_HAS_TIMEDJOIN
inline int pthread_timedjoin_np(pthread_t thread, void **retval, const struct timespec *abstime) {
    return EINVAL;
}
#endif // !defined(PTHREAD_HAS_TIMEDJOIN)
#endif // defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )

#if !defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)
#if !defined(NO_DEFINE_PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)
#define PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP
inline int pthread_timedjoin_relative_np(pthread_t thread, void **retval, const struct timespec *reltime) {
#if defined(PTHREAD_HAS_TIMEDJOIN)
    if ((reltime)) {
        int err = 0; struct timespec untilTime;
        if (!(err = ::clock_gettime(CLOCK_REALTIME, &untilTime))) {
            untilTime.tv_sec +=  reltime->tv_sec;
            untilTime.tv_nsec +=  reltime->tv_nsec;
            err = ::pthread_timedjoin_np(thread, retval, &untilTime);
        }
        return err;
    }
#endif /// defined(PTHREAD_HAS_TIMEDJOIN)
    return EINVAL;
}
#endif // !defined(NO_DEFINE_PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)
#endif // !defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)

namespace xos {
namespace mt {
namespace posix {

///////////////////////////////////////////////////////////////////////
///  Class: ThreadT
///////////////////////////////////////////////////////////////////////
template 
<typename TError = int, TError VErrorSuccess = 0, TError VErrorFailed = -1,
 typename TAttached = pthread_t*, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::ThreadT<TAttached, TUnattached, VUnattached>,
 class TThread = extended::ThreadT
 <TError, VErrorSuccess, VErrorFailed, TAttached, TUnattached, VUnattached, TImplement>,
 class TImplements = TImplement, class TExtends = TThread>

class _EXPORTED_ ThreadT: virtual public TImplements, public TExtends {
public:
    typedef TImplements Implements;
    typedef TExtends Extends;

    typedef typename Implements::Ran Ran;

    typedef typename Extends::Error Error;
    static const Error ErrorSuccess = Extends::ErrorSuccess;
    static const Error ErrorFailed = Extends::ErrorFailed;

    typedef typename Extends::Attached Attached;
    static const typename Extends::UnattachedT Unattached = Extends::Unattached;

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ThreadT(Attached detached, bool isCreated, bool isLogged, bool isErrLogged): Extends(detached, isCreated, isLogged, isErrLogged) {
    }
    ThreadT(Attached detached, bool isCreated, bool isLogged): Extends(detached, isCreated, isLogged) {
    }
    ThreadT(Attached detached, bool isCreated): Extends(detached, isCreated) {
    }
    ThreadT(Attached detached): Extends(detached) {
    }
    ThreadT(bool isLogged, bool isErrLogged): Extends(isLogged, isErrLogged) {
        XOS_MT_THREAD_CREATED();
    }
    ThreadT(bool isLogged): Extends(isLogged) {
        XOS_MT_THREAD_CREATED();
    }
    ThreadT(Ran& ran): Extends(ran) {
        XOS_MT_THREAD_CREATED();
    }
    ThreadT() {
        XOS_MT_THREAD_CREATED();
    }
    virtual ~ThreadT() {
        XOS_MT_THREAD_DESTROYED();
    }
private:
    ThreadT(const ThreadT& copy) {
        XOS_MT_THREAD_UNEXPECTED();
    }

public:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Attached CreateAttached(bool initiallySuspended) {
        Attached detached = ((Attached)Unattached);
        if (!(initiallySuspended)) {
            detached = CreateAttached();
        }
        return detached;
    }
    virtual Attached CreateAttached() {
        Attached detached = ((Attached)Unattached);
        pthread_t& thread = _thread;
        pthread_attr_t threadAttr;
        IS_ERR_LOGGED_DEBUG("CreateDetached(threadAttr, thread)...");
        if ((detached = CreateDetached(threadAttr, thread))) {
            this->Attach(detached);
        }
        return detached;
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual JoinStatus TryJoinDetached(bool& forked, Attached detached) const { 
#if defined(PTHREAD_HAS_TRYJOIN)
        if (((Attached)Unattached) != (detached)) {
            void* value = 0;
            int err = 0;

            IS_ERR_LOGGED_TRACE("::pthread_tryjoin_np(*detached, &value)...");
            if (!(err = ::pthread_tryjoin_np(*detached, &value))) {
                forked = false;
                return JoinSuccess;
            } else {
                switch(err) {
                case EBUSY:
                    IS_ERR_LOGGED_TRACE("...EBUSY err = "<< err << " on ::pthread_tryjoin_np(*detached, &value)");
                    return JoinBusy;
                case ETIMEDOUT:
                    IS_ERR_LOGGED_TRACE("...ETIMEDOUT err = "<< err << " on ::pthread_tryjoin_np(*detached, &value)");
                    return JoinBusy;
                case EINTR:
                    IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_tryjoin_np(*detached, &value)");
                    return JoinInterrupted;
                default:
                    IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_tryjoin_np(*detached, &value)");
                    forked = false;
                    return JoinFailed;
                }
            }
        }
        return JoinFailed; 
#else /// defined(PTHREAD_HAS_TRYJOIN)
        IS_ERR_LOGGED_ERROR("...return JoinInvalid");
        return JoinInvalid;
#endif /// defined(PTHREAD_HAS_TRYJOIN)
    }
    virtual JoinStatus TimedJoinDetached(bool& forked, Attached detached, mseconds_t milliseconds) const { 
        if (0 < (milliseconds)) {
#if defined(PTHREAD_HAS_TIMEDJOIN)
            if (((Attached)Unattached) != (detached)) {
                const char* _relative_np = "_np";
                mseconds_t millisecondsThreashold = this->TimedLoggedThreasholdMilliseconds();
                bool isLogged = ((this->IsLogged()) && (milliseconds >= millisecondsThreashold));
                void* value = 0;
                int err = 0;
                struct timespec untilTime;

#if defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)
                _relative_np = "_relative_np";
                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "::memset(&untilTime, 0, sizeof(untilTime))...");
                ::memset(&untilTime, 0, sizeof(untilTime));
#else /// defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)
                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "::clock_gettime(CLOCK_REALTIME, &untilTime);...");
                ::clock_gettime(CLOCK_REALTIME, &untilTime);
#endif /// defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)

                untilTime.tv_sec +=  MSecondsSeconds(milliseconds);
                untilTime.tv_nsec +=  MSecondsNSeconds(MSecondsMSeconds(milliseconds));

                IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "::pthread_timedjoin" << _relative_np << "(*detached, &value, &untilTime)...");
#if defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)
                err = ::pthread_timedjoin_relative_np(*detached, &value, &untilTime);
#else /// defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)
                err = ::pthread_timedjoin_np(*detached, &value, &untilTime);
#endif /// defined(PTHREAD_HAS_TIMEDJOIN_RELATIVE_NP)

                if (!(err)) {
                    IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...::pthread_timedjoin" << _relative_np << "(*detached, &value, &untilTime)");
                    forked = false;
                    return JoinSuccess;
                } else {
                    switch(err) {
                    case EBUSY:
                        IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...EBUSY err = "<< err << " on ::pthread_timedjoin" << _relative_np << "(*detached, &value, &untilTime)");
                        return JoinBusy;
                    case ETIMEDOUT:
                        IF_ERR_LOGGED_DEBUG_TRACE(isLogged, isLogged, "...ETIMEDOUT err = "<< err << " on ::pthread_timedjoin" << _relative_np << "(*detached, &value, &untilTime)");
                        return JoinBusy;
                    case EINTR:
                        IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_timedjoin" << _relative_np << "(*detached, &value, &untilTime)");
                        return JoinInterrupted;
                    default:
                        IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_timedjoin" << _relative_np << "(*detached, &value, &untilTime)");
                        forked = false;
                        return JoinFailed;
                    }
                }
            }
#else /// defined(PTHREAD_HAS_TIMEDJOIN)
            IS_ERR_LOGGED_ERROR("...return JoinInvalid");
            return JoinInvalid;
#endif /// defined(PTHREAD_HAS_TIMEDJOIN)
        } else {
            if (0 > (milliseconds)) {
                return UntimedJoinDetached(forked, detached);
            } else {
                return TryJoinDetached(forked, detached);
            }
        }
        return JoinFailed; 
    }
    virtual JoinStatus UntimedJoinDetached(bool& forked, Attached detached) const { 
        if (((Attached)Unattached) != (detached)) {
            void* value = 0;
            int err = 0;
            IS_ERR_LOGGED_DEBUG("::pthread_join(*detached, &value)...");
            if (!(err = ::pthread_join(*detached, &value))) {
                IS_ERR_LOGGED_DEBUG("...::pthread_join(*detached, &value)");
                forked = false;
                return JoinSuccess;
            } else {
                switch(err) {
                case EBUSY:
                    IS_ERR_LOGGED_ERROR("...EBUSY err = " << err << " on ::pthread_join(*detached, &value)");
                    return JoinBusy;
                case ETIMEDOUT:
                    IS_ERR_LOGGED_ERROR("...ETIMEDOUT err = " << err << " on ::pthread_join(*detached, &value)");
                    return JoinBusy;
                case EINTR:
                    IS_ERR_LOGGED_ERROR("...EINTR err = " << err << " on ::pthread_join(*detached, &value)");
                    return JoinInterrupted;
                default:
                    IS_ERR_LOGGED_ERROR("...failed err = " << err << " on ::pthread_join(*detached, &value)");
                    forked = false;
                }
            }
        }
        return JoinFailed; 
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual Attached CreateDetached(pthread_attr_t& threadAttr, pthread_t& thread, bool initiallySuspended) const {
        Attached detached = ((Attached)Unattached);
        if (!(initiallySuspended)) {
            detached = CreateDetached(threadAttr, thread);
        }
        return detached;
    }
    virtual Attached CreateDetached(pthread_attr_t& threadAttr, pthread_t& thread) const {
        Attached detached = ((Attached)Unattached);
        int err = 0;

        IS_ERR_LOGGED_DEBUG("::pthread_attr_init(&threadAttr)...");
        if ((err = ::pthread_attr_init(&threadAttr))) {
            IS_ERR_LOGGED_ERROR("...failed " << err << " on ::pthread_attr_init(&threadAttr).");
        } else {

            IS_ERR_LOGGED_DEBUG("::pthread_create(&thread, &threadAttr, StartRoutine, ((void*)this))...");
            if ((err = ::pthread_create(&thread, &threadAttr, StartRoutine, ((void*)this)))) {
                IS_ERR_LOGGED_ERROR("...failed " << err << " on ::pthread_create(&thread, &threadAttr, StartRoutine, ((void*)this))");
            } else {

                IS_ERR_LOGGED_DEBUG("::pthread_attr_destroy(&threadAttr)...");
                if ((err = ::pthread_attr_destroy(&threadAttr))) {
                    bool forked = true;
                    IS_ERR_LOGGED_ERROR("...failed " << err << " on ::pthread_attr_destroy(&threadAttr).");
                    UntimedJoinDetached(forked, &thread);
                    return detached;
                }
                detached = &thread;
                return detached;
            }
            IS_ERR_LOGGED_DEBUG("::pthread_attr_destroy(&threadAttr)...");
            if ((err = ::pthread_attr_destroy(&threadAttr))) {
                IS_ERR_LOGGED_ERROR("...failed " << err << " on ::pthread_attr_destroy(&threadAttr).");
            }
        }
        return detached;
    }
    virtual bool DestroyDetached(Attached detached) const {
        IS_ERR_LOGGED_DEBUG("(&_thread != detached)...");
        if ((&_thread != detached)) {
            IS_ERR_LOGGED_ERROR("...failed (&_thread != detached)");
            return false;
        }
        IS_ERR_LOGGED_DEBUG("...!(&_thread != detached)");
        return true;
    }

protected:
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    static void* StartRoutine(void* param) {
        void* result = 0;
        ThreadT* t = 0;
        if ((t = ((ThreadT*)param))) {
            IF_ERR_LOGGED_DEBUG(t->IsLogged(), t->IsErrLogged(), "t->Run()...");
            t->Run();
            IF_ERR_LOGGED_DEBUG(t->IsLogged(), t->IsErrLogged(), "...t->Run()");
        } else {
            LOG_ERROR("...unexpected 0 = ((ThreadT*)param)");
        }
        return result;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
protected:
    pthread_t _thread;
}; /// class _EXPORTED_ ThreadT
typedef ThreadT<> Thread;

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_POSIX_THREAD_HXX_
