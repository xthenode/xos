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
//
// pthread_tryjoin_np
//
#if !defined(PTHREAD_HAS_TRYJOIN)
#define PTHREAD_HAS_TRYJOIN
#if !defined(pthread_tryjoin_np)
#define pthread_tryjoin_np(t, v) EINVAL
#endif // !defined(pthread_tryjoin_np)
#endif // !defined(PTHREAD_HAS_TRYJOIN)
//
// pthread_timedjoin_np
//
#if !defined(PTHREAD_HAS_TIMEDJOIN)
#define PTHREAD_HAS_TIMEDJOIN
#if !defined(pthread_timedjoin_np)
#define pthread_timedjoin_np(t, v, u) EINVAL
#endif // !defined(pthread_timedjoin_np)
#endif // !defined(PTHREAD_HAS_TIMEDJOIN)
#endif // defined(_POSIX_TIMEOUTS) && (_POSIX_TIMEOUTS >=0 )

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

class _EXPORT_CLASS ThreadT: virtual public TImplements, public TExtends {
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
                void* value = 0;
                int err = 0;
                struct timespec untilTime;

                IS_ERR_LOGGED_TRACE("::clock_gettime(CLOCK_REALTIME, &untilTime);...");
                ::clock_gettime(CLOCK_REALTIME, &untilTime);
                untilTime.tv_sec +=  MSecondsSeconds(milliseconds);
                untilTime.tv_nsec +=  MSecondsNSeconds(MSecondsMSeconds(milliseconds));

                IS_ERR_LOGGED_TRACE("::pthread_timedjoin_np(*detached, &value, &untilTime)...");
                if (!(err = ::pthread_timedjoin_np(*detached, &value, &untilTime))) {
                    forked = false;
                    return JoinSuccess;
                } else {
                    switch(err) {
                    case EBUSY:
                        IS_ERR_LOGGED_TRACE("...EBUSY err = "<< err << " on ::pthread_timedjoin_np(*detached, &value, &untilTime)");
                        return JoinBusy;
                    case ETIMEDOUT:
                        IS_ERR_LOGGED_TRACE("...ETIMEDOUT err = "<< err << " on ::pthread_timedjoin_np(*detached, &value, &untilTime)");
                        return JoinBusy;
                    case EINTR:
                        IS_ERR_LOGGED_ERROR("...EINTR err = "<< err << " on ::pthread_timedjoin_np(*detached, &value, &untilTime)");
                        return JoinInterrupted;
                    default:
                        IS_ERR_LOGGED_ERROR("...failed err = "<< err << " on ::pthread_timedjoin_np(*detached, &value, &untilTime)");
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
}; /// class _EXPORT_CLASS ThreadT
typedef ThreadT<> Thread;

} /// namespace posix
} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_POSIX_THREAD_HXX_
